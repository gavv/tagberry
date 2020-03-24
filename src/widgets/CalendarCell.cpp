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
#include <QLocale>
#include <QPainter>

namespace tagberry::widgets {

CalendarCell::CalendarCell(QWidget* parent, int row, int col)
    : QWidget(parent)
    , m_row(row)
    , m_col(col)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    m_layout.addWidget(&m_cell);
    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));

    setLayout(&m_layout);

    m_headLayout.addWidget(&m_month);
    m_headLayout.addWidget(&m_day);
    m_headLayout.setContentsMargins(QMargins(5, 3, 5, 3));

    m_cell.setRowLayout(Row_Header, &m_headLayout);

    QFont font;
    font.setPointSize(10);
    m_day.setFont(font);

    font.setBold(true);
    m_month.setFont(font);

    m_month.setAlignment(Qt::AlignLeft);
    m_day.setAlignment(Qt::AlignRight);

    m_month.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_day.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    connect(&m_cell, &MultirowCell::clicked, [=] { clicked(this); });

    updateTextColors();
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
    return m_cell.getRowLayout(Row_Body);
}

void CalendarCell::setContentLayout(QLayout* layout)
{
    m_cell.setRowLayout(Row_Body, layout, 1);
}

void CalendarCell::setMonth(int month)
{
    m_month.setText(month >= 1 ? QLocale().monthName(month) : "");
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
    updateTextColors();
}

void CalendarCell::setToday(bool today)
{
    if (today == m_isToday) {
        return;
    }
    m_isToday = today;
    updateCellColors();
}

void CalendarCell::setColors(QHash<QString, QColor> colors)
{
    m_normalTextColor = colors["text"];
    m_dimmedTextColor = colors["text-dimmed"];

    m_normalBackgroundColor = colors["background"];
    m_todayBackgroundColor = colors["background-today"];

    m_borderColor = colors["border"];

    updateCellColors();
    updateTextColors();
}

void CalendarCell::updateCellColors()
{
    m_cell.setBorderColor(m_borderColor);
    m_cell.setSeparatorColor(m_borderColor);
    m_cell.setRowColor(
        Row_Header, m_isToday ? m_todayBackgroundColor : m_normalBackgroundColor);
    m_cell.setRowColor(Row_Body, m_normalBackgroundColor);
}

void CalendarCell::updateTextColors()
{
    auto style
        = QString("QLabel { color: %1; }")
              .arg(m_isDimmed ? m_dimmedTextColor.name() : m_normalTextColor.name());

    m_month.setStyleSheet(style);
    m_day.setStyleSheet(style);
}

} // namespace tagberry::widgets
