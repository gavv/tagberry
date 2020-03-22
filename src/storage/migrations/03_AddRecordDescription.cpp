/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "storage/migrations/03_AddRecordDescription.hpp"

// QSqlMigrator
#include <api.h>

using namespace Commands;
using namespace Structure;

namespace tagberry::storage {

M03_AddRecordDescription::M03_AddRecordDescription()
{
    auto col = Column("description", Type("NVARCHAR(10000)"));

    add(new AddColumn(col, "records"));
}

} // namespace tagberry::storage
