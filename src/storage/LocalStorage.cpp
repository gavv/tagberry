/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "storage/LocalStorage.hpp"

#include <QDebug>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>

namespace tagberry::storage {

namespace {

QString dbPath()
{
    auto config = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    if (!config.isEmpty()) {
        return QDir(config).filePath("tagberry-qt");
    }
    return QDir(QDir::homePath()).filePath(".tagberry-qt");
}

} // namespace

bool LocalStorage::open()
{
    auto path = dbPath();

    qDebug() << "opening" << path;

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
    QSqlQuery query;

    if (!m_db.tables().contains(QLatin1String("tags"))) {
        qDebug() << "creating table 'tags'";
        if (!query.exec("CREATE TABLE tags ("
                        "id INT PRIMARY KEY, "
                        "name NVARCHAR(100))")) {
            return false;
        }
    }

    if (!m_db.tables().contains(QLatin1String("records"))) {
        qDebug() << "creating table 'records'";
        if (!query.exec("CREATE TABLE records ("
                        "id INT PRIMARY KEY, "
                        "date INT, "
                        "title NVARCHAR(1000))")) {
            return false;
        }
    }

    if (!m_db.tables().contains(QLatin1String("record2tag"))) {
        qDebug() << "creating table 'record2tag'";
        if (!query.exec("CREATE TABLE record2tag (record INT, tag INT)")) {
            return false;
        }
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

    if (!saveRecordBody(record)) {
        QSqlDatabase::database().rollback();
        return false;
    }

    QSqlDatabase::database().commit();

    record->unsetDirty();
    return true;
}

bool LocalStorage::saveRecordBody(models::RecordPtr record)
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
            "UPDATE records SET title = (:name), date = (:date) WHERE id = (:id)");
        query.bindValue(":id", record->id());
    } else {
        query.prepare("INSERT INTO records (title, date) VALUES (:title, :date)");
    }

    query.bindValue(":title", record->title());
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
    auto indexRecDate = recQuery.record().indexOf("date");
    auto indexRecTitle = recQuery.record().indexOf("title");

    while (recQuery.next()) {
        auto rec = recDir.getOrCreateRecord(recQuery.value(indexRecID).toString());

        QDateTime dt;
        dt.setTime_t(recQuery.value(indexRecDate).toInt());

        rec->setDate(dt.date());
        rec->setTitle(recQuery.value(indexRecTitle).toString());

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
