/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "storage/migrations/01_CreateTables.hpp"

// QSqlMigrator
#include <api.h>

using namespace Commands;
using namespace Structure;

namespace tagberry::storage {

M01_CreateTables::M01_CreateTables()
{
    {
        Table::Builder table("tags");

        table << Column("id", Type("INTEGER"), Column::Primary | Column::AutoIncrement)
              << Column("name", Type("NVARCHAR(100)"));

        add(new CreateTable(table));
    }
    {
        Table::Builder table("records");

        table << Column("id", Type("INTEGER"), Column::Primary | Column::AutoIncrement)
              << Column("date", Type("INTEGER"))
              << Column("title", Type("NVARCHAR(1000)"));

        add(new CreateTable(table));
    }
    {
        Table::Builder table("record2tag");

        table << Column("record", Type("INTEGER")) << Column("tag", Type("INTEGER"));

        add(new CreateTable(table));
    }
}

} // namespace tagberry::storage
