/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "models/RecordSet.hpp"

namespace tagberry::models {

QList<RecordPtr> RecordSet::getRecords() const
{
    return m_records;
}

void RecordSet::addRecord(RecordPtr rec)
{
    if (m_records.indexOf(rec) != -1) {
        return;
    }

    connect(rec.get(), &Record::tagsChanged, this, &RecordSet::recordTagsChanged);

    m_records.append(rec);

    recordListChanged();
    recordTagsChanged();
}

void RecordSet::removeRecord(RecordPtr rec)
{
    if (m_records.indexOf(rec) == -1) {
        return;
    }

    disconnect(rec.get(), nullptr, this, nullptr);

    m_records.removeAll(rec);

    recordListChanged();
    recordTagsChanged();
}

void RecordSet::clearRecords()
{
    for (auto rec : m_records) {
        disconnect(rec.get(), nullptr, this, nullptr);
    }

    m_records.clear();

    recordListChanged();
    recordTagsChanged();
}

QList<TagPtr> RecordSet::getAllTags() const
{
    QList<TagPtr> tags;
    for (auto rec : m_records) {
        for (auto tag : rec->tags()) {
            if (tags.indexOf(tag) == -1) {
                tags.append(tag);
            }
        }
    }
    return tags;
}

int RecordSet::numRecordsWithTag(TagPtr tag)
{
    int ret = 0;
    for (auto rec : m_records) {
        if (rec->hasTag(tag)) {
            ret++;
        }
    }
    return ret;
}

} // namespace tagberry::models
