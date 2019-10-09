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
#include "storage/LocalStorage.hpp"
#include "widgets/TagCounterCalendar.hpp"

#include <QDate>
#include <QHBoxLayout>
#include <QMainWindow>

namespace tagberry::presenters {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(storage::LocalStorage& storage);

private slots:
    void setFocusedTag(widgets::TagCounter*);

    void refreshCalendar();
    void refreshCalendarCell(QDate date);

    void connectRecordSets(QDate from, QDate to);

private:
    storage::LocalStorage& m_storage;

    QHBoxLayout* m_layout;
    QWidget* m_widget;

    widgets::TagCounterCalendar* m_tagCalendarWidget;

    models::TagsDirectory* m_tagDir;
    models::RecordsDirectory* m_recDir;
};

} // tagberry::presenters
