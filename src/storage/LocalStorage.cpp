/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "storage/LocalStorage.hpp"

namespace tagberry::storage {

bool LocalStorage::readPage(const QPair<QDate, QDate> range,
    models::RecordsDirectory& recDir, models::TagsDirectory& tagDir)
{
    // FIXME: implement and use sqlite backend

    auto t1 = tagDir.getOrCreateTag("tag-1");
    auto t2 = tagDir.getOrCreateTag("tag-2");
    auto t3 = tagDir.getOrCreateTag("tag-3");

    auto r1 = recDir.getOrCreateRecord("recID1");
    auto r2 = recDir.getOrCreateRecord("recID2");

    r1->setDate(range.first);
    r2->setDate(range.second);

    r1->addTag(t1);
    r1->addTag(t2);

    r2->addTag(t2);
    r2->addTag(t3);

    return true;
}

} // namespace tagberry::storage
