/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "models/ColorScheme.hpp"
#include "models/RecordsDirectory.hpp"
#include "models/TagsDirectory.hpp"

#include <QDate>
#include <QPair>

namespace tagberry::models {

class Root : public QObject {
    Q_OBJECT

public:
    Root();

    ColorScheme& colorScheme();

    TagsDirectory& tags();

    RecordsDirectory& currentPage();

    QPair<QDate, QDate> currentPageRange();

    void resetCurrentPage(QPair<QDate, QDate>);

    QDate currentDate();
    void setCurrentDate(QDate);

signals:
    void currentPageChanged();
    void currentDateChanged(QDate);

private:
    ColorScheme m_colorScheme;
    TagsDirectory m_tags;
    RecordsDirectory m_currentPageRecords;

    QPair<QDate, QDate> m_currentPageRange;
    QDate m_currentDate;
};

} // namespace tagberry::models
