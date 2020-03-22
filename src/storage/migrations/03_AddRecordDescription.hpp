/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <Migrations/Migration.h>

namespace tagberry::storage {

class M03_AddRecordDescription : public Migrations::Migration {
public:
    M03_AddRecordDescription();
};

} // namespace tagberry::storage
