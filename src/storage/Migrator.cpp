/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "storage/Migrator.hpp"

#include "storage/migrations/01_CreateTables.hpp"
#include "storage/migrations/02_AddRecordState.hpp"

#include <QDebug>

#include <Migrations/MigrationRepository.h>
#include <QSqlMigrator/QSqlMigratorService.h>
#include <SqliteMigrator/SqliteMigrator.h>

#include <LocalSchemeMigrator/LocalSchemeComparisonContext.h>
#include <LocalSchemeMigrator/LocalSchemeComparisonService.h>
#include <LocalSchemeMigrator/LocalSchemeMigrator.h>
#include <MigrationExecution/LocalSchemeMigrationExecutionContext.h>
#include <MigrationExecution/LocalSchemeMigrationExecutionService.h>

namespace tagberry::storage {

Migrator::Migrator(QSqlDatabase& db)
    : m_db(db)
{
    m_migrations.insert("M01_CreateTables", new M01_CreateTables());
    m_migrations.insert("M02_AddRecordState", new M02_AddRecordState());
}

Migrator::~Migrator()
{
    for (auto m : m_migrations) {
        delete m;
    }
}

bool Migrator::migrate()
{
    qDebug() << "applying migrations";

    auto contextBuilder
        = MigrationExecution::MigrationExecutionContext::Builder(m_migrations);

    contextBuilder.setDatabase(m_db);

    auto context = SqliteMigrator::buildContext(contextBuilder);

    QSqlMigrator::QSqlMigratorService manager;
    return manager.applyAll(*context);
}

bool Migrator::validate()
{
    qDebug() << "validating schema";

    LocalSchemePtr localScheme(new Structure::LocalScheme);

    auto localContext
        = MigrationExecution::LocalSchemeMigrationExecutionContext(m_migrations);

    localContext.setLocalScheme(localScheme);
    localContext.setLocalSchemeCommandServiceRepository(
        LocalSchemeMigrator::createCommandServiceRepository());

    MigrationExecution::LocalSchemeMigrationExecutionService localManager;
    localManager.executeBatch(localContext.migrationMap().keys(), localContext);

    auto contextBuilder
        = MigrationExecution::MigrationExecutionContext::Builder(m_migrations);

    contextBuilder.setDatabase(m_db);

    auto context = SqliteMigrator::buildContext(contextBuilder);

    LocalSchemeMigrator::LocalSchemeComparisonContext comparisonContext(
        localScheme, context->helperRepository(), m_db);

    LocalSchemeMigrator::LocalSchemeComparisonService comparisonService;

    return comparisonService.compareLocalSchemeWithDatabase(comparisonContext);
}

} // namespace tagberry::storage
