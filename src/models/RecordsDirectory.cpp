/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "models/RecordsDirectory.hpp"

namespace tagberry::models {

RecordsDirectory::RecordsDirectory(QObject* parent)
    : QObject(parent)
    , m_recordWithoutDate(new RecordSet(this))
{
}

RecordSet* RecordsDirectory::recordsByDate(const QDate& date)
{
    auto recSet = m_recordsByDate[date];

    if (!recSet) {
        recSet = new RecordSet(this);
        m_recordsByDate[date] = recSet;
    }

    return recSet;
}

RecordSet* RecordsDirectory::recordsWithoutDate()
{
    if (!m_recordWithoutDate) {
        m_recordWithoutDate = new RecordSet(this);
    }
    return m_recordWithoutDate;
}

Record* RecordsDirectory::getOrCreateRecord(const QString& id)
{
    auto rec = m_recordsById[id];

    if (!rec) {
        rec = new Record(id, this);

        connect(rec, &Record::dateChanged, this, &RecordsDirectory::recordDateChanged);

        m_recordsById[id] = rec;
        recordsWithoutDate()->addRecord(rec);
    }

    return rec;
}

void RecordsDirectory::removeRecord(Record* rec)
{
    if (rec->date() == QDate()) {
        recordsWithoutDate()->removeRecord(rec);
    } else {
        recordsByDate(rec->date())->removeRecord(rec);
    }

    m_recordsById.remove(rec->id());
}

void RecordsDirectory::clearRecords()
{
    m_recordWithoutDate->clearRecords();
    delete m_recordWithoutDate;
    m_recordWithoutDate = NULL;

    for (auto it = m_recordsByDate.begin(); it != m_recordsByDate.end();
         it = m_recordsByDate.erase(it)) {
        it.value()->clearRecords();
        delete it.value();
    }

    for (auto it = m_recordsById.begin(); it != m_recordsById.end();
         it = m_recordsById.erase(it)) {
        delete it.value();
    }
}

void RecordsDirectory::recordDateChanged(QDate oldDate, QDate newDate)
{
    auto rec = qobject_cast<Record*>(sender());

    if (oldDate == QDate()) {
        recordsWithoutDate()->removeRecord(rec);
    } else {
        recordsByDate(oldDate)->removeRecord(rec);
    }

    recordsByDate(newDate)->addRecord(rec);
}

} // tagberry::models
