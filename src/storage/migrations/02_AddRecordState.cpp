/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "storage/migrations/02_AddRecordState.hpp"

// QSqlMigrator
#include <api.h>

using namespace Commands;
using namespace Structure;

namespace tagberry::storage {

M02_AddRecordState::M02_AddRecordState()
{
    auto col = Column("state", Type("INTEGER"), "1");

    add(new AddColumn(col, "records"));
}

} // namespace tagberry::storage
