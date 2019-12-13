/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "presenters/MainWindow.hpp"

namespace tagberry::presenters {

MainWindow::MainWindow(storage::LocalStorage& storage)
    : m_storage(storage)
    , m_layout(new QHBoxLayout)
    , m_widget(new QWidget(this))
{
    m_storage.readAllTags(m_root.tags());

    m_calendarArea = new CalendarArea(m_storage, m_root);
    m_recordsArea = new RecordsArea(m_storage, m_root);

    m_layout->setContentsMargins(QMargins(4, 0, 10, 0));

    m_layout->addWidget(m_calendarArea, 4);
    m_layout->addWidget(m_recordsArea, 1);

    m_widget->setLayout(m_layout);

    setCentralWidget(m_widget);
    setWindowTitle("Tagberry");

    connect(m_calendarArea, &CalendarArea::focusTaken, m_recordsArea,
        &RecordsArea::clearFocus);

    m_calendarArea->setFocus();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    m_recordsArea->setHeaderHeight(m_calendarArea->headerHeight());
}

} // namespace tagberry::presenters
