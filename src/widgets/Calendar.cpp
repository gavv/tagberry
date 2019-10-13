/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/Calendar.hpp"

#include <QDebug>

namespace tagberry::widgets {

Calendar::Calendar(QWidget* parent)
    : QWidget(parent)
    , m_switch(new CalendarSwitch)
    , m_normalColor("#ffffff")
    , m_todayColor("#c2efa5")
    , m_year(-1)
    , m_month(-1)
    , m_offset(-1)
    , m_weekStart(-1)
{
    setLayout(&m_calendarLayout);

    m_calendarLayout.addLayout(&m_switchLayout, 0);
    m_calendarLayout.addLayout(&m_head, 0);
    m_calendarLayout.addLayout(&m_grid, 1);

    m_switchLayout.addStretch(1);
    m_switchLayout.addWidget(m_switch);
    m_switchLayout.addStretch(1);

    for (int col = 0; col < columnCount(); col++) {
        m_head.addSpacing(3);
        m_head.addWidget((m_days[col] = new QLabel));

        for (int row = 0; row < rowCount(); row++) {
            auto cell = new CalendarCell(nullptr, row, col);

            connect(cell, &CalendarCell::clicked, this, &Calendar::setFocus);

            m_grid.addWidget(cell, row, col);
        }
    }

    connect(m_switch, &CalendarSwitch::switched, this, &Calendar::setPage);
    connect(m_switch, &CalendarSwitch::today, this, &Calendar::setToday);

    setWeekStart(Qt::Monday);
}

QList<QDate> Calendar::getSelectedDates() const
{
    QList<QDate> ret;

    for (auto cell : m_focused) {
        if (cell) {
            ret.append(getDate(cell->row(), cell->column()));
        }
    }

    return ret;
}

QPair<QDate, QDate> Calendar::getVisibleRange() const
{
    return qMakePair(getDate(0, 0), getDate(rowCount() - 1, columnCount() - 1));
}

int Calendar::rowCount() const
{
    return NumWeeks;
}

int Calendar::columnCount() const
{
    return NumDays;
}

int Calendar::headerHeight()
{
    return m_switch->height() + m_days[0]->height() + 8;
}

void Calendar::setWeekStart(Qt::DayOfWeek day)
{
    for (int col = 0; col < columnCount(); col++) {
        m_days[col]->setText(QDate::shortDayName((day + col - 1) % NumDays + 1));
    }
    m_weekStart = day;
}

void Calendar::setPage(int year, int month)
{
    if (year == m_year && month == m_month) {
        return;
    }

    if (year < 1970 || year > 9999) {
        qCritical() << "bad year value, should be in range [1970; 9999]";
        return;
    }

    if (month < 1 || month > 12) {
        qCritical() << "bad month value, should be in range [1; 12]";
        return;
    }

    m_year = year;
    m_month = month;

    const int firstDay = QDate(m_year, m_month, 1).dayOfWeek();
    if (firstDay >= m_weekStart) {
        m_offset = (firstDay - m_weekStart);
    } else {
        m_offset = (firstDay - m_weekStart + NumDays);
    }

    updateCells();
    m_switch->setYearMonth(m_year, m_month);

    pageChanged();
}

void Calendar::setDate(const QDate& date)
{
    setPage(date.year(), date.month());
    setFocus(getCell(date));
}

void Calendar::setToday()
{
    setDate(QDate::currentDate());
}

void Calendar::setNormalColor(const QColor& color)
{
    m_normalColor = color;
    updateCells();
}

void Calendar::setTodayColor(const QColor& color)
{
    m_todayColor = color;
    updateCells();
}

void Calendar::setFocus(CalendarCell* newCell)
{
    for (auto cell : m_focused) {
        if (cell) {
            cell->setFocused(false);
        }
    }

    m_focused.clear();

    if (newCell) {
        newCell->setFocused(true);
        m_focused.append(newCell);
    }

    focusChanged(newCell);
}

QDate Calendar::getDate(int row, int col) const
{
    return QDate(m_year, m_month, 1).addDays(row * NumDays + col - m_offset);
}

CalendarCell* Calendar::getCell(int row, int col)
{
    if (QLayoutItem* item = m_grid.itemAtPosition(row, col)) {
        if (QWidget* widget = item->widget()) {
            return static_cast<CalendarCell*>(widget);
        }
    }
    qCritical() << "invalid calendar row/col" << row << col;
    return nullptr;
}

CalendarCell* Calendar::getCell(const QDate& date)
{
    const QDate start = getDate(0, 0);
    const qint64 diff = start.daysTo(date);

    if (diff < 0 || diff >= rowCount() * columnCount()) {
        return nullptr;
    } else {
        const int row = int(diff / columnCount());
        const int col = int(diff % columnCount());
        return getCell(row, col);
    }
}

void Calendar::updateCells()
{
    for (int col = 0; col < columnCount(); col++) {
        for (int row = 0; row < rowCount(); row++) {
            auto cell = getCell(row, col);
            if (!cell) {
                continue;
            }

            QDate date = getDate(row, col);

            cell->setMonth(date.day() == 1 ? date.month() : -1);
            cell->setDay(date.day());
            cell->setDimmed(date.month() != m_month);

            cell->setHeaderColor(
                date == QDate::currentDate() ? m_todayColor : m_normalColor);
        }
    }
}

} // namespace tagberry::widgets
