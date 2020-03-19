/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <Migrations/MigrationRepository.h>

#include <QSqlDatabase>

namespace tagberry::storage {

class Migrator {
public:
    Migrator(QSqlDatabase&);
    ~Migrator();

    bool migrate();
    bool validate();

private:
    Migrations::MigrationRepository::NameMigrationMap m_migrations;
    QSqlDatabase m_db;
};

} // namespace tagberry::storage
