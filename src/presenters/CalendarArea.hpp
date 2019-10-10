/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include "models/ColorScheme.hpp"
#include "models/RecordsDirectory.hpp"
#include "models/TagsDirectory.hpp"
#include "storage/LocalStorage.hpp"
#include "widgets/TagCalendar.hpp"

#include <QHBoxLayout>
#include <QWidget>

namespace tagberry::presenters {

class CalendarArea : public QWidget {
    Q_OBJECT

public:
    CalendarArea(storage::LocalStorage& storage, models::TagsDirectory& tagDir,
        models::RecordsDirectory& recDir);

private slots:
    void refreshPage();

    void clearFocus();
    void changeFocus(widgets::TagLabel*);

private:
    void rebuildCell(QDate);

private:
    QHBoxLayout* m_layout;
    widgets::TagCalendar* m_calendar;

    storage::LocalStorage& m_storage;

    models::TagsDirectory& m_tagDir;
    models::RecordsDirectory& m_recDir;
};

} // namespace tagberry::presenters
