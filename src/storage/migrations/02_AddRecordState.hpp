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

class M02_AddRecordState : public Migrations::Migration {
public:
    M02_AddRecordState();
};

} // namespace tagberry::storage
