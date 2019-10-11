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
    , m_headFrame(new QFrame)
    , m_bodyFrame(new QFrame)
    , m_bgColor("#ffffff")
    , m_borderColor("#767C82")
    , m_headerColor(m_bgColor)
    , m_isFocused(false)
{
    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout.setSpacing(0);

    m_layout.addWidget(m_headFrame, 0);
    m_layout.addWidget(m_bodyFrame, 1);

    setLayout(&m_layout);

    m_headFrame->setContentsMargins(QMargins(0, 0, 0, 0));
    m_bodyFrame->setContentsMargins(QMargins(0, 0, 0, 0));

    m_headFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

QLayout* Cell::headLayout()
{
    return m_headFrame->layout();
}

void Cell::setHeadLayout(QLayout* layout)
{
    m_headFrame->setLayout(layout);
}

QLayout* Cell::bodyLayout()
{
    return m_bodyFrame->layout();
}

void Cell::setBodyLayout(QLayout* layout)
{
    m_bodyFrame->setLayout(layout);
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

void Cell::setHeaderColor(QColor header)
{
    if (m_headerColor == header) {
        return;
    }
    m_headerColor = header;
    repaint();
}

void Cell::paintEvent(QPaintEvent* event)
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

void Cell::mousePressEvent(QMouseEvent*)
{
    clicked();
}

} // namespace tagberry::widgets
