/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/MultirowCell.hpp"

#include <QApplication>
#include <QDate>
#include <QDebug>
#include <QPainter>

namespace tagberry::widgets {

MultirowCell::MultirowCell(QWidget* parent)
    : QWidget(parent)
{
    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout.setSpacing(0);

    setLayout(&m_layout);
}

QLayout* MultirowCell::getRowLayout(int index)
{
    if (index >= m_rowFrames.size()) {
        return nullptr;
    }
    return m_rowFrames[index]->layout();
}

void MultirowCell::setRowLayout(int index, QLayout* layout, int stretch)
{
    auto frame = new QFrame;

    frame->setLayout(layout);
    frame->setContentsMargins(QMargins(0, 0, 0, 0));
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    frame->setStyleSheet("QFrame { background-color: transparent; }");

    if (m_rowFrames.size() <= index) {
        m_rowFrames.resize(index + 1);
        m_rowColors.resize(index + 1);
    } else {
        m_layout.removeItem(m_layout.itemAt(index));
    }

    m_rowFrames[index] = frame;
    m_rowColors[index] = "#ffffff";
    m_layout.insertWidget(index, frame, stretch);
}

void MultirowCell::setFocused(bool focused)
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

void MultirowCell::setRowColor(int index, QColor color)
{
    if (index >= m_rowColors.size()) {
        qCritical() << "row index out of range";
        return;
    }
    if (m_rowColors[index] == color) {
        return;
    }
    m_rowColors[index] = color;
    repaint();
}

void MultirowCell::setBorderColor(QColor color)
{
    if (m_borderColor == color) {
        return;
    }
    m_borderColor = color;
    repaint();
}

void MultirowCell::paintEvent(QPaintEvent* event)
{
    QPainter pt(this);

    pt.setRenderHint(QPainter::Qt4CompatiblePainting, true);

    const int borderWidth1 = m_isFocused ? 1 : 0;
    const int borderWidth2 = borderWidth1 + 1;

    int off = 0;

    for (int n = 0; n < m_rowFrames.size(); n++) {
        if (!m_rowFrames[n]) {
            continue;
        }

        pt.setRenderHint(QPainter::Antialiasing, m_isFocused);
        pt.setPen(m_rowColors[n]);
        pt.setBrush(m_rowColors[n]);
        pt.drawRoundedRect(
            QRect(1, off + 1, width() - 1, height() - 1), borderWidth2, borderWidth2);

        off += m_rowFrames[n]->height();

        pt.setRenderHint(QPainter::Antialiasing, false);
        pt.setPen(QPen(m_borderColor, 1));
        pt.setBrush(QBrush());
        pt.drawLine(QLine(0, off, width(), off));

        off += 1;
    }

    pt.setRenderHint(QPainter::Antialiasing, m_isFocused);
    pt.setPen(QPen(m_borderColor, borderWidth2));
    pt.setBrush(QBrush());
    pt.drawRoundedRect(QRect(borderWidth1, borderWidth1, width() - borderWidth2,
                           height() - borderWidth2),
        borderWidth2, borderWidth2);

    QWidget::paintEvent(event);
}

void MultirowCell::mousePressEvent(QMouseEvent*)
{
    clicked();
}

} // namespace tagberry::widgets
