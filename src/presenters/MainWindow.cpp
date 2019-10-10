/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "presenters/MainWindow.hpp"

namespace tagberry::presenters {

MainWindow::MainWindow(storage::LocalStorage& storage)
    : m_storage(storage)
    , m_layout(new QHBoxLayout)
    , m_widget(new QWidget(this))
    , m_calendarArea(new CalendarArea(m_storage, m_root))
{
    m_layout->addWidget(m_calendarArea);
    m_layout->setContentsMargins(QMargins(0, 0, 0, 0));

    m_widget->setLayout(m_layout);
    setCentralWidget(m_widget);
}

} // namespace tagberry::presenters
