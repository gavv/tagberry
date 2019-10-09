/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "widgets/TagCounterCalendar.hpp"
#include "widgets/FlowLayout.hpp"

namespace tagberry::widgets {

TagCounterCalendar::TagCounterCalendar(QWidget* parent)
    : QWidget(parent)
    , m_layout(new QHBoxLayout)
    , m_calendar(new Calendar)
{
    m_layout->setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout->addWidget(m_calendar);

    setLayout(m_layout);

    connect(m_calendar, &Calendar::pageChanged, this, &TagCounterCalendar::pageChanged);
    connect(m_calendar, &Calendar::focusChanged, this, &TagCounterCalendar::focusCleared);

    for (int row = 0; row < m_calendar->rowCount(); row++) {
        for (int col = 0; col < m_calendar->columnCount(); col++) {
            auto cell = m_calendar->getCell(row, col);

            cell->setContentLayout(new FlowLayout);
            cell->contentLayout()->setContentsMargins(QMargins(6, 6, 6, 6));
        }
    }
}

QPair<QDate, QDate> TagCounterCalendar::getVisibleRange() const
{
    return m_calendar->getVisibleRange();
}

QList<TagCounter*> TagCounterCalendar::getTags(const QDate& date)
{
    auto cell = m_calendar->getCell(date);
    if (!cell) {
        return {};
    }

    QList<TagCounter*> ret;

    for (int pos = 0; pos < cell->contentLayout()->count(); pos++) {
        if (auto item = cell->contentLayout()->itemAt(pos)) {
            if (auto tag = static_cast<TagCounter*>(item->widget())) {
                ret.append(tag);
            }
        }
    }

    return ret;
}

void TagCounterCalendar::addTag(const QDate& date, TagCounter* tag)
{
    auto cell = m_calendar->getCell(date);
    if (!cell) {
        return;
    }

    cell->contentLayout()->addWidget(tag);

    connect(tag, &TagCounter::clicked, [=] {
        m_calendar->setFocus(cell);
        focusChanged(tag);
    });
}

void TagCounterCalendar::removeTag(const QDate& date, TagCounter* tag)
{
    auto cell = m_calendar->getCell(date);
    if (!cell) {
        return;
    }

    cell->contentLayout()->removeWidget(tag);

    delete tag;
}

void TagCounterCalendar::clearTags(const QDate& date)
{
    auto cell = m_calendar->getCell(date);
    if (!cell) {
        return;
    }

    removeCellTags(cell);
}

void TagCounterCalendar::clearTags()
{
    for (int row = 0; row < m_calendar->rowCount(); row++) {
        for (int col = 0; col < m_calendar->columnCount(); col++) {
            removeCellTags(m_calendar->getCell(row, col));
        }
    }
}

void TagCounterCalendar::removeCellTags(CalendarCell* cell)
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
