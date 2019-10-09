/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include "models/Record.hpp"
#include "models/RecordSet.hpp"

#include <QList>
#include <QObject>

namespace tagberry::models {

class RecordsDirectory : public QObject {
    Q_OBJECT

public:
    explicit RecordsDirectory(QObject* parent = nullptr);

    RecordSet* recordsByDate(const QDate&);

    RecordSet* recordsWithoutDate();

    Record* getOrCreateRecord(const QString& id);

    void removeRecord(Record*);

    void clearRecords();

private slots:
    void recordDateChanged(QDate oldDate, QDate newDate);

private:
    QHash<QString, Record*> m_recordsById;

    QHash<QDate, RecordSet*> m_recordsByDate;

    RecordSet* m_recordWithoutDate;
};

} // tagberry::models
