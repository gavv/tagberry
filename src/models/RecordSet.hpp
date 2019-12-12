/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "models/Record.hpp"

#include <QList>
#include <QObject>
#include <QString>

#include <memory>

namespace tagberry::models {

class RecordSet : public QObject, public std::enable_shared_from_this<RecordSet> {
    Q_OBJECT

public:
    QList<RecordPtr> getRecords() const;

    void addRecord(RecordPtr);
    void removeRecord(RecordPtr);

    void clearRecords();

    QList<TagPtr> getAllTags() const;

    int numRecordsWithTag(TagPtr);

    bool checkAllRecordsWithTagComplete(TagPtr);

signals:
    void recordListChanged();
    void recordTagsChanged();
    void recordStatesChanged();

private:
    void notifyChanged();

    QList<RecordPtr> m_records;
};

using RecordSetPtr = std::shared_ptr<RecordSet>;

} // namespace tagberry::models
