/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
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
    , m_month(new QLabel)
    , m_day(new QLabel)
    , m_headFrame(new QFrame)
    , m_bodyFrame(new QFrame)
    , m_bgColor("#ffffff")
    , m_borderColor("#767C82")
    , m_headerColor(m_bgColor)
    , m_normalTextColor("#000000")
    , m_dimmedTextColor("#999999")
    , m_isFocused(false)
    , m_isDimmed(false)
{
    m_cellLayout.addWidget(m_headFrame, 0);
    m_cellLayout.addWidget(m_bodyFrame, 1);
    setLayout(&m_cellLayout);

    m_headLayout.addWidget(m_month);
    m_headLayout.addWidget(m_day);

    m_headFrame->setLayout(&m_headLayout);

    m_headLayout.setContentsMargins(QMargins(5, 3, 5, 3));
    m_cellLayout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_cellLayout.setSpacing(0);

    m_headFrame->setContentsMargins(QMargins(0, 0, 0, 0));
    m_bodyFrame->setContentsMargins(QMargins(0, 0, 0, 0));

    m_headFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QFont font;
    font.setBold(true);
    m_month->setFont(font);

    m_month->setAlignment(Qt::AlignLeft);
    m_day->setAlignment(Qt::AlignRight);

    m_month->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_day->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

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
    return m_bodyFrame->layout();
}

void CalendarCell::setContentLayout(QLayout* layout)
{
    m_bodyFrame->setLayout(layout);
}

void CalendarCell::setMonth(int month)
{
    m_month->setText(month >= 1 ? QDate::shortMonthName(month) : "");
}

void CalendarCell::setDay(int day)
{
    m_day->setText(QString("%1").arg(day));
}

void CalendarCell::setFocused(bool focused)
{
    if (focused == m_isFocused) {
        return;
    }
    m_isFocused = focused;
    repaint();
    if (m_isFocused) {
        if (auto widget = qApp->focusWidget()) {
            widget->clearFocus();
        }
    }
}

void CalendarCell::setDimmed(bool dimmed)
{
    if (dimmed == m_isDimmed) {
        return;
    }
    m_isDimmed = dimmed;
    updateTextColor();
}

void CalendarCell::setBackgroundColor(const QColor& color)
{
    if (m_bgColor == color) {
        return;
    }
    m_bgColor = color;
    repaint();
}

void CalendarCell::setBorderColor(const QColor& color)
{
    if (m_borderColor == color) {
        return;
    }
    m_borderColor = color;
    repaint();
}

void CalendarCell::setHeaderColor(const QColor& color)
{
    if (m_headerColor == color) {
        return;
    }
    m_headerColor = color;
    repaint();
}

void CalendarCell::setTextColor(const QColor& normal, const QColor& dimmed)
{
    if (m_normalTextColor == normal && m_dimmedTextColor == dimmed) {
        return;
    }
    m_normalTextColor = normal;
    m_dimmedTextColor = dimmed;
    updateTextColor();
}

void CalendarCell::paintEvent(QPaintEvent* event)
{
    QPainter pt(this);

    pt.setRenderHint(QPainter::Qt4CompatiblePainting, true);

    if (m_isFocused) {
        pt.setRenderHint(QPainter::Antialiasing, true);
    }

    const int f1 = m_isFocused ? 1 : 0;
    const int f2 = f1 + 1;
    const int hf = m_headFrame->height();

    pt.setPen(m_headerColor);
    pt.setBrush(m_headerColor);
    pt.drawRoundedRect(QRect(1, 1, width() - 1, height() - 1), f2, f2);

    pt.setPen(m_bgColor);
    pt.setBrush(m_bgColor);
    pt.drawRoundedRect(QRect(1, hf + 1, width() - 1, height() - 1), f2, f2);

    pt.setPen(QPen(m_borderColor, f2));
    pt.setBrush(QBrush());
    pt.drawRoundedRect(QRect(f1, f1, width() - f2, height() - f2), f2, f2);

    if (m_isFocused) {
        pt.setRenderHint(QPainter::Antialiasing, false);
    }

    pt.setPen(QPen(m_borderColor, 1));
    pt.drawLine(QLine(0, hf, width(), hf));

    QWidget::paintEvent(event);
}

void CalendarCell::mousePressEvent(QMouseEvent*)
{
    clicked(this);
}

void CalendarCell::updateTextColor()
{
    auto style
        = QString("QLabel { color: %1; }")
              .arg(m_isDimmed ? m_dimmedTextColor.name() : m_normalTextColor.name());

    m_month->setStyleSheet(style);
    m_day->setStyleSheet(style);
}

} // namespace tagberry::widgets
