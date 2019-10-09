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

#include <QList>
#include <QObject>
#include <QString>

namespace tagberry::models {

class RecordSet : public QObject {
    Q_OBJECT

public:
    explicit RecordSet(QObject* parent = nullptr);

    QList<Record*> getRecords() const;

    void addRecord(Record*);
    void removeRecord(Record*);

    void clearRecords();

    QList<Tag*> getAllTags() const;

    int numRecordsWithTag(Tag*);

signals:
    void recordListChanged();
    void recordTagsChanged();

private:
    QList<Record*> m_records;
};

} // tagberry::models
