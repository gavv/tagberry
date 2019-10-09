/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "widgets/TagCounter.hpp"

#include <QFontMetrics>
#include <QPainter>

namespace tagberry::widgets {

TagCounter::TagCounter(QWidget* parent)
    : QWidget(parent)
    , m_fgRegular(0x3c, 0x3c, 0x3c)
    , m_fgFocused(0xb0, 0xb0, 0xb0)
    , m_bg(0xff, 0xff, 0xff)
    , m_font("monospace", 8)
    , m_hPad(4)
    , m_vPad(2)
    , m_rounding(1)
    , m_cntSize(0)
    , m_w(0)
    , m_h(0)
    , m_isFocused(false)
    , m_isChecked(false)
{
}

const QString& TagCounter::text() const
{
    return m_text;
}

void TagCounter::setText(QString text)
{
    if (m_text == text) {
        return;
    }
    m_text = text;
    updateSize();
    repaint();
}

void TagCounter::setCounter(int counter)
{
    QString newCounter = QString("%1").arg(counter);
    if (m_counter == newCounter) {
        return;
    }
    m_counter = newCounter;
    updateSize();
    repaint();
}

void TagCounter::setFocused(bool focused)
{
    if (m_isFocused == focused) {
        return;
    }
    m_isFocused = focused;
    repaint();
}

void TagCounter::setChecked(bool checked)
{
    if (m_isChecked == checked) {
        return;
    }
    m_isChecked = checked;
    repaint();
}

void TagCounter::setForegroundColor(const QColor& regular, const QColor& focused)
{
    if (m_fgRegular == regular && m_fgFocused == focused) {
        return;
    }
    m_fgRegular = regular;
    m_fgFocused = focused;
    repaint();
}

void TagCounter::setBackgroundColor(const QColor& color)
{
    if (m_bg == color) {
        return;
    }
    m_bg = color;
    repaint();
}

void TagCounter::setFont(const QFont& font)
{
    if (m_font == font) {
        return;
    }
    m_font = font;
    updateSize();
    repaint();
}

void TagCounter::setPadding(int h, int v)
{
    if (m_hPad == h && m_vPad == v) {
        return;
    }
    m_hPad = h;
    m_vPad = v;
    updateSize();
    repaint();
}

void TagCounter::setRounding(int r)
{
    if (m_rounding == r) {
        return;
    }
    m_rounding = r;
    repaint();
}

void TagCounter::updateSize()
{
    QFontMetrics metrics(m_font);

    m_cntSize = metrics.width(m_counter);

    m_w = metrics.width(m_text) + m_cntSize + m_hPad * 4;
    m_h = metrics.height() + m_vPad * 2;

    setFixedWidth(m_w + 1);
    setFixedHeight(m_h + 1);
}

void TagCounter::paintEvent(QPaintEvent*)
{
    QPainter pt(this);

    pt.setRenderHint(QPainter::Qt4CompatiblePainting, true);

    if (m_isFocused) {
        pt.setPen(QPen(m_fgFocused, 1));
        pt.setBrush(QBrush(m_fgFocused));
    } else {
        pt.setPen(QPen(m_bg, 1));
        pt.setBrush(QBrush(m_bg));
    }

    pt.drawRoundedRect(QRect(0, 0, m_w, m_h), m_rounding, m_rounding);

    if (m_isChecked && !m_isFocused) {
        pt.setPen(QPen(m_fgRegular, 1));
        pt.setBrush(QBrush(m_fgRegular));

        pt.drawRoundedRect(
            QRect(m_w - m_cntSize - m_hPad * 2, 0, m_cntSize + m_hPad * 2, m_h),
            m_rounding, m_rounding);
    } else {
        pt.setPen(QPen(m_fgRegular, 1));
        pt.drawLine(
            QLine(m_w - m_cntSize - m_hPad * 2, 0, m_w - m_cntSize - m_hPad * 2, m_h));
    }

    pt.setPen(QPen(m_fgRegular, 1));
    pt.setBrush(QBrush());
    pt.drawRoundedRect(QRect(0, 0, m_w, m_h), m_rounding, m_rounding);

    pt.setFont(m_font);

    if (m_isFocused) {
        pt.setPen(QPen(m_bg, 1));
    } else {
        pt.setPen(QPen(m_fgRegular, 1));
    }

    pt.drawText(QRect(0, 0, m_w - m_cntSize - m_hPad * 2, m_h), Qt::AlignCenter, m_text);

    if (m_isChecked || m_isFocused) {
        pt.setPen(QPen(m_bg, 1));
    } else {
        pt.setPen(QPen(m_fgRegular, 1));
    }

    pt.drawText(QRect(m_w - m_cntSize - m_hPad * 2, 0, m_cntSize + m_hPad * 2, m_h),
        Qt::AlignCenter, m_counter);
}

void TagCounter::mousePressEvent(QMouseEvent*)
{
    clicked();
}

} // tagberry::widgets
