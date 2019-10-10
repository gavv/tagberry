/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "widgets/TagCalendar.hpp"
#include "widgets/FlowLayout.hpp"

namespace tagberry::widgets {

TagCalendar::TagCalendar(QWidget* parent)
    : QWidget(parent)
    , m_layout(new QHBoxLayout)
    , m_calendar(new Calendar)
{
    m_layout->setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout->addWidget(m_calendar);

    setLayout(m_layout);

    connect(m_calendar, &Calendar::pageChanged, this, &TagCalendar::pageChanged);
    connect(m_calendar, &Calendar::focusChanged, this, &TagCalendar::focusCleared);

    for (int row = 0; row < m_calendar->rowCount(); row++) {
        for (int col = 0; col < m_calendar->columnCount(); col++) {
            auto cell = m_calendar->getCell(row, col);

            cell->setContentLayout(new FlowLayout);
            cell->contentLayout()->setContentsMargins(QMargins(6, 6, 6, 6));
        }
    }
}

QPair<QDate, QDate> TagCalendar::getVisibleRange() const
{
    return m_calendar->getVisibleRange();
}

void TagCalendar::addTag(const QDate& date, TagLabel* tag)
{
    auto cell = m_calendar->getCell(date);
    if (!cell) {
        return;
    }

    cell->contentLayout()->addWidget(tag);

    connect(tag, &TagLabel::clicked, [=] {
        m_calendar->setFocus(cell);
        focusChanged(tag);
    });
}

void TagCalendar::clearTags(const QDate& date)
{
    auto cell = m_calendar->getCell(date);
    if (!cell) {
        return;
    }

    removeCellTags(cell);
}

void TagCalendar::clearTags()
{
    for (int row = 0; row < m_calendar->rowCount(); row++) {
        for (int col = 0; col < m_calendar->columnCount(); col++) {
            removeCellTags(m_calendar->getCell(row, col));
        }
    }
}

void TagCalendar::removeCellTags(CalendarCell* cell)
{
    if (!cell) {
        return;
    }

    while (cell->contentLayout()->count()) {
        if (auto item = cell->contentLayout()->itemAt(0)) {
            cell->contentLayout()->removeItem(item);

            delete item->widget();
            delete item;
        }
    }
}

} // tagberry::widgets
