/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include "models/RecordsDirectory.hpp"
#include "models/TagsDirectory.hpp"

#include <QDate>

namespace tagberry::storage {

class LocalStorage {
public:
    bool readPage(const QPair<QDate, QDate> range, models::RecordsDirectory& recDir,
        models::TagsDirectory& tagDir);
};

} // namespace tagberry::storage
