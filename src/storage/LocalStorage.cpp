/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "storage/LocalStorage.hpp"
#include "storage/Migrator.hpp"

#include <QDebug>
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>

namespace tagberry::storage {

namespace {

std::unique_ptr<QLockFile> lockDB(QString path)
{
    auto lockPath = path + ".lock";

    qDebug() << "locking" << lockPath;

    auto lock = std::make_unique<QLockFile>(lockPath);

    if (!lock->tryLock()) {
        return nullptr;
    }

    return lock;
}

bool backupDB(QString path)
{
    if (!QFile::exists(path)) {
        return true;
    }

    auto bakPath = path + ".bak";

    qDebug() << "copying" << path << "to" << bakPath;

    if (QFile::exists(bakPath)) {
        QFile::remove(bakPath);
    }

    return QFile::copy(path, bakPath);
}

} // namespace

bool LocalStorage::open(const QString& path)
{
    qDebug() << "opening" << path;

    if (!(m_lock = lockDB(path))) {
        qCritical() << "can't acquire exclusive lock on DB";
        return false;
    }

    if (!backupDB(path)) {
        qCritical() << "can't make backup";
        return false;
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qCritical() << "can't open" << path;
        return false;
    }

    if (!initTables()) {
        qCritical() << "can't initialize tables";
        return false;
    }

    return true;
}

bool LocalStorage::initTables()
{
    Migrator m(m_db);

    if (!m.migrate()) {
        qCritical() << "can't apply migrations";
        return false;
    }

    if (!m.validate()) {
        qCritical() << "can't validate db schema";
        return false;
    }

    return true;
}

bool LocalStorage::saveTag(models::TagPtr tag)
{
    if (!tag->isDirty()) {
        return true;
    }

    QSqlQuery query;

    if (tag->hasID()) {
        query.prepare("UPDATE tags SET name = (:name) WHERE id = (:id)");
        query.bindValue(":id", tag->id());
    } else {
        query.prepare("INSERT INTO tags (name) VALUES (:name)");
    }

    query.bindValue(":name", tag->name());

    if (!query.exec()) {
        qCritical() << "can't write tag";
        return false;
    }

    if (!tag->hasID()) {
        tag->setID(query.lastInsertId().toString());
    }
    tag->unsetDirty();

    return true;
}

bool LocalStorage::saveRecord(models::RecordPtr record)
{
    if (!record->isDirty()) {
        return true;
    }

    for (auto tag : record->tags()) {
        if (!saveTag(tag)) {
            return false;
        }
    }

    QSqlDatabase::database().transaction();

    if (!saveRecordImp(record)) {
        QSqlDatabase::database().rollback();
        return false;
    }

    QSqlDatabase::database().commit();

    record->unsetDirty();
    return true;
}

bool LocalStorage::saveRecordImp(models::RecordPtr record)
{
    QSqlQuery query;

    if (record->hasID()) {
        query.prepare("DELETE FROM record2tag WHERE record = (:id)");
        query.bindValue(":id", record->id());

        if (!query.exec()) {
            qCritical() << "can't delete record2tag";
            return false;
        }
    }

    if (record->hasID()) {
        query.prepare(
            "UPDATE records SET title = (:title), state = (:state), date = (:date) "
            "WHERE id = (:id)");
        query.bindValue(":id", record->id());
    } else {
        query.prepare("INSERT INTO records (title, state, date) "
                      "VALUES (:title, :state, :date)");
    }

    query.bindValue(":title", record->title());
    query.bindValue(":state", record->complete() ? 1 : 0);
    query.bindValue(":date", QDateTime(record->date()).toTime_t());

    if (!query.exec()) {
        qCritical() << "can't write record";
        return false;
    }

    if (!record->hasID()) {
        record->setID(query.lastInsertId().toString());
    }

    for (auto tag : record->tags()) {
        query.prepare("INSERT INTO record2tag (record, tag) VALUES (:record, :tag)");
        query.bindValue(":record", record->id());
        query.bindValue(":tag", tag->id());

        if (!query.exec()) {
            qCritical() << "can't insert record2tag";
            return false;
        }
    }

    return true;
}

bool LocalStorage::removeRecord(models::RecordPtr record)
{
    if (!record->hasID()) {
        return true;
    }

    QSqlDatabase::database().transaction();

    if (!removeRecordImp(record)) {
        QSqlDatabase::database().rollback();
        return false;
    }

    QSqlDatabase::database().commit();

    return true;
}

bool LocalStorage::removeRecordImp(models::RecordPtr record)
{
    QSqlQuery query;

    query.prepare("DELETE FROM record2tag WHERE record = (:record)");
    query.bindValue(":record", record->id());

    if (!query.exec()) {
        qCritical() << "can't remove from record2tag";
        return false;
    }

    query.prepare("DELETE FROM records WHERE id = (:id)");
    query.bindValue(":id", record->id());

    if (!query.exec()) {
        qCritical() << "can't remove from records";
        return false;
    }

    return true;
}

bool LocalStorage::readAllTags(models::TagsDirectory& tagDir)
{
    QSqlQuery query;

    if (!query.exec("SELECT * from tags")) {
        qCritical() << "can't read tags";
        return false;
    }

    auto indexTagID = query.record().indexOf("id");
    auto indexTagName = query.record().indexOf("name");

    while (query.next()) {
        auto tag = tagDir.getOrCreateTag(query.value(indexTagID).toString());

        tag->setName(query.value(indexTagName).toString());
        tag->unsetDirty();
    }

    return true;
}

bool LocalStorage::readPage(const QPair<QDate, QDate> range,
    models::RecordsDirectory& recDir, models::TagsDirectory& tagDir)
{
    QSqlQuery recQuery;

    recQuery.prepare("SELECT * from records WHERE date >= (:from) AND date <= (:to)");
    recQuery.bindValue(":from", QDateTime(range.first).toTime_t());
    recQuery.bindValue(":to", QDateTime(range.second).toTime_t());

    if (!recQuery.exec()) {
        return false;
    }

    auto indexRecID = recQuery.record().indexOf("id");
    auto indexRecTitle = recQuery.record().indexOf("title");
    auto indexRecState = recQuery.record().indexOf("state");
    auto indexRecDate = recQuery.record().indexOf("date");

    while (recQuery.next()) {
        auto rec = recDir.getOrCreateRecord(recQuery.value(indexRecID).toString());

        rec->setTitle(recQuery.value(indexRecTitle).toString());
        rec->setComplete(recQuery.value(indexRecState).toInt() == 1);

        QDateTime dt;
        dt.setTime_t(recQuery.value(indexRecDate).toInt());
        rec->setDate(dt.date());

        QSqlQuery tagQuery;

        tagQuery.prepare(
            "SELECT * from tags INNER JOIN record2tag ON tags.id = record2tag.tag "
            "WHERE record2tag.record = (:record)");

        tagQuery.bindValue(":record", rec->id());

        if (!tagQuery.exec()) {
            return false;
        }

        auto indexTagID = tagQuery.record().indexOf("id");
        auto indexTagName = tagQuery.record().indexOf("name");

        while (tagQuery.next()) {
            auto tag = tagDir.getOrCreateTag(tagQuery.value(indexTagID).toString());

            tag->setName(tagQuery.value(indexTagName).toString());
            tag->unsetDirty();

            rec->addTag(tag);
        }

        rec->unsetDirty();
    }

    return true;
}

} // namespace tagberry::storage
