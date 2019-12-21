/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/Cell.hpp"

#include <QApplication>
#include <QDate>
#include <QPainter>

namespace tagberry::widgets {

Cell::Cell(QWidget* parent)
    : QWidget(parent)
    , m_headerFrame(new QFrame)
    , m_bodyFrame(new QFrame)
    , m_footerFrame(new QFrame)
    , m_isFocused(false)
{
    setColors("#ffffff", "#ffffff", "#000000");

    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout.setSpacing(0);

    m_layout.addWidget(m_headerFrame, 0);
    m_layout.addWidget(m_bodyFrame, 1);
    m_layout.addWidget(m_footerFrame, 0);

    setLayout(&m_layout);

    m_headerFrame->setContentsMargins(QMargins(0, 0, 0, 0));
    m_bodyFrame->setContentsMargins(QMargins(0, 0, 0, 0));
    m_footerFrame->setContentsMargins(QMargins(0, 0, 0, 0));

    m_headerFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_footerFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_headerFrame->setStyleSheet("QFrame { background-color: transparent; }");
    m_bodyFrame->setStyleSheet("QFrame { background-color: transparent; }");
    m_footerFrame->setStyleSheet("QFrame { background-color: transparent; }");
}

QLayout* Cell::headerLayout()
{
    return m_headerFrame->layout();
}

void Cell::setHeaderLayout(QLayout* layout)
{
    m_headerFrame->setLayout(layout);
}

QLayout* Cell::bodyLayout()
{
    return m_bodyFrame->layout();
}

void Cell::setBodyLayout(QLayout* layout)
{
    m_bodyFrame->setLayout(layout);
}

QLayout* Cell::footerLayout()
{
    return m_footerFrame->layout();
}

void Cell::setFooterLayout(QLayout* layout)
{
    m_footerFrame->setLayout(layout);
}

void Cell::setFocused(bool focused)
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

void Cell::setColors(QColor headerBackground, QColor bodyBackground, QColor border)
{
    if (m_headerColor == headerBackground && m_bodyColor == bodyBackground
        && m_borderColor == border) {
        return;
    }

    m_headerColor = headerBackground;
    m_bodyColor = bodyBackground;
    m_borderColor = border;

    repaint();
}

void Cell::paintEvent(QPaintEvent* event)
{
    QPainter pt(this);

    pt.setRenderHint(QPainter::Qt4CompatiblePainting, true);

    if (m_isFocused) {
        pt.setRenderHint(QPainter::Antialiasing, true);
    }

    const int border1 = m_isFocused ? 1 : 0;
    const int border2 = border1 + 1;

    const int headerOff = m_headerFrame->height();
    const int footerOff = height() - m_footerFrame->height();

    pt.setPen(m_headerColor);
    pt.setBrush(m_headerColor);
    pt.drawRoundedRect(QRect(1, 1, width() - 1, height() - 1), border2, border2);

    pt.setPen(m_bodyColor);
    pt.setBrush(m_bodyColor);
    pt.drawRoundedRect(QRect(1, headerOff + 1, width() - 1, height() - 1), border2, border2);

    pt.setPen(QPen(m_borderColor, border2));
    pt.setBrush(QBrush());
    pt.drawRoundedRect(
        QRect(border1, border1, width() - border2, height() - border2), border2, border2);

    if (m_isFocused) {
        pt.setRenderHint(QPainter::Antialiasing, false);
    }

    pt.setPen(QPen(m_borderColor, 1));
    pt.drawLine(QLine(0, headerOff, width(), headerOff));
    pt.drawLine(QLine(0, footerOff, width(), footerOff));

    QWidget::paintEvent(event);
}

void Cell::mousePressEvent(QMouseEvent*)
{
    clicked();
}

} // namespace tagberry::widgets
