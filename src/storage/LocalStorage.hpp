/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "models/RecordsDirectory.hpp"
#include "models/TagsDirectory.hpp"

#include <QDate>
#include <QSqlDatabase>

namespace tagberry::storage {

class LocalStorage {
public:
    bool open(const QString& path);

    bool saveTag(models::TagPtr tag);

    bool saveRecord(models::RecordPtr record);

    bool removeRecord(models::RecordPtr record);

    bool readAllTags(models::TagsDirectory& tagDir);

    bool readPage(const QPair<QDate, QDate> range, models::RecordsDirectory& recDir,
        models::TagsDirectory& tagDir);

private:
    bool initTables();

    bool saveRecordImp(models::RecordPtr record);
    bool removeRecordImp(models::RecordPtr record);

    QSqlDatabase m_db;
};

} // namespace tagberry::storage
