/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <Migrations/Migration.h>

namespace tagberry::storage {

class M01_CreateTables : public Migrations::Migration {
public:
    M01_CreateTables();
};

} // namespace tagberry::storage
