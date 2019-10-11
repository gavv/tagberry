/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "models/RecordsDirectory.hpp"

namespace tagberry::models {

RecordSetPtr RecordsDirectory::recordsByDate(const QDate& date)
{
    auto recSet = m_recordsByDate[date];

    if (!recSet) {
        recSet = std::make_shared<RecordSet>();
        m_recordsByDate[date] = recSet;
    }

    return recSet;
}

RecordSetPtr RecordsDirectory::recordsWithoutDate()
{
    if (!m_recordWithoutDate) {
        m_recordWithoutDate = std::make_shared<RecordSet>();
    }
    return m_recordWithoutDate;
}

RecordPtr RecordsDirectory::createRecord()
{
    auto rec = std::make_shared<Record>();

    connect(rec.get(), &Record::idChanged, this, &RecordsDirectory::recordIdChanged);
    connect(rec.get(), &Record::dateChanged, this, &RecordsDirectory::recordDateChanged);

    m_records.insert(rec);
    recordsWithoutDate()->addRecord(rec);

    return rec;
}

RecordPtr RecordsDirectory::getOrCreateRecord(const QString& id)
{
    if (auto rec = m_recordByID[id]) {
        return rec;
    }

    auto rec = createRecord();
    rec->setID(id);

    return rec;
}

void RecordsDirectory::removeRecord(RecordPtr rec)
{
    disconnect(rec.get(), nullptr, this, nullptr);

    if (rec->date() == QDate()) {
        recordsWithoutDate()->removeRecord(rec);
    } else {
        recordsByDate(rec->date())->removeRecord(rec);
    }

    if (rec->hasID()) {
        m_recordByID.remove(rec->id());
    }

    m_records.erase(rec);
}

void RecordsDirectory::clearRecords()
{
    for (auto rec : m_records) {
        disconnect(rec.get(), nullptr, this, nullptr);
    }

    m_recordByID.clear();
    m_recordsByDate.clear();
    m_recordWithoutDate.reset();
    m_records.clear();
}

void RecordsDirectory::recordIdChanged(QString id)
{
    auto record = qobject_cast<Record*>(sender());

    for (auto it = m_recordByID.begin(); it != m_recordByID.end(); ++it) {
        if (it.value().get() == record) {
            m_recordByID.erase(it);
            break;
        }
    }

    if (!id.isEmpty()) {
        m_recordByID[id] = record->shared_from_this();
    }
}

void RecordsDirectory::recordDateChanged(QDate oldDate, QDate newDate)
{
    auto rec = qobject_cast<Record*>(sender());

    if (oldDate == QDate()) {
        recordsWithoutDate()->removeRecord(rec->shared_from_this());
    } else {
        recordsByDate(oldDate)->removeRecord(rec->shared_from_this());
    }

    recordsByDate(newDate)->addRecord(rec->shared_from_this());
}

} // namespace tagberry::models
