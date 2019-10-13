/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "models/Root.hpp"
#include "storage/LocalStorage.hpp"
#include "widgets/TagCalendar.hpp"

#include <QHBoxLayout>
#include <QWidget>

namespace tagberry::presenters {

class CalendarArea : public QWidget {
    Q_OBJECT

public:
    CalendarArea(storage::LocalStorage& storage, models::Root& root);

    int headerHeight();

private slots:
    void refreshPage();

    void changeCurrentDate(QDate);

    void clearTagFocus();
    void changeTagFocus(widgets::TagLabel*);

private:
    void rebuildCell(QDate);

private:
    QHBoxLayout* m_layout;
    widgets::TagCalendar* m_calendar;

    storage::LocalStorage& m_storage;

    models::Root& m_root;
};

} // namespace tagberry::presenters
