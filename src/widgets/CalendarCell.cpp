/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/CalendarCell.hpp"

#include <QApplication>
#include <QDate>
#include <QPainter>

namespace tagberry::widgets {

CalendarCell::CalendarCell(QWidget* parent, int row, int col)
    : QWidget(parent)
    , m_row(row)
    , m_col(col)
    , m_normalTextColor("#000000")
    , m_dimmedTextColor("#999999")
    , m_isDimmed(false)
{
    m_layout.addWidget(&m_cell);
    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));

    setLayout(&m_layout);

    m_headLayout.addWidget(&m_month);
    m_headLayout.addWidget(&m_day);
    m_headLayout.setContentsMargins(QMargins(5, 3, 5, 3));

    m_cell.setHeadLayout(&m_headLayout);

    QFont font;
    font.setBold(true);
    m_month.setFont(font);

    m_month.setAlignment(Qt::AlignLeft);
    m_day.setAlignment(Qt::AlignRight);

    m_month.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_day.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    connect(&m_cell, &Cell::clicked, [=] { clicked(this); });

    updateTextColor();
}

int CalendarCell::row() const
{
    return m_row;
}

int CalendarCell::column() const
{
    return m_col;
}

QLayout* CalendarCell::contentLayout()
{
    return m_cell.bodyLayout();
}

void CalendarCell::setContentLayout(QLayout* layout)
{
    m_cell.setBodyLayout(layout);
}

void CalendarCell::setMonth(int month)
{
    m_month.setText(month >= 1 ? QDate::shortMonthName(month) : "");
}

void CalendarCell::setDay(int day)
{
    m_day.setText(QString("%1").arg(day));
}

void CalendarCell::setFocused(bool focused)
{
    m_cell.setFocused(focused);
}

void CalendarCell::setDimmed(bool dimmed)
{
    if (dimmed == m_isDimmed) {
        return;
    }
    m_isDimmed = dimmed;
    updateTextColor();
}

void CalendarCell::setHeaderColor(QColor color)
{
    m_cell.setHeaderColor(color);
}

void CalendarCell::updateTextColor()
{
    auto style
        = QString("QLabel { color: %1; }")
              .arg(m_isDimmed ? m_dimmedTextColor.name() : m_normalTextColor.name());

    m_month.setStyleSheet(style);
    m_day.setStyleSheet(style);
}

} // namespace tagberry::widgets
