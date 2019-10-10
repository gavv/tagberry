/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "models/Root.hpp"

namespace tagberry::models {

Root::Root()
{
    m_tags.setColorScheme(&m_colorScheme);
}

ColorScheme& Root::colorScheme()
{
    return m_colorScheme;
}

TagsDirectory& Root::tags()
{
    return m_tags;
}

RecordsDirectory& Root::currentPage()
{
    return m_currentPageRecords;
}

QPair<QDate, QDate> Root::currentPageRange()
{
    return m_currentPageRange;
}

void Root::resetCurrentPage(QPair<QDate, QDate> range)
{
    if (m_currentPageRange == range) {
        return;
    }

    m_currentPageRange = range;
    m_currentPageRecords.clearRecords();

    currentPageChanged();
}

QDate Root::currentDate()
{
    return m_currentDate;
}

void Root::setCurrentDate(QDate date)
{
    if (m_currentDate == date) {
        return;
    }
    m_currentDate = date;
    currentDateChanged();
}

} // namespace tagberry::models
