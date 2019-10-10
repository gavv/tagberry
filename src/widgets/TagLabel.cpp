/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/TagLabel.hpp"

#include <QFontMetrics>
#include <QPainter>

namespace tagberry::widgets {

TagLabel::TagLabel(QWidget* parent)
    : QWidget(parent)
    , m_closeIndicator(false)
    , m_fgRegular(0x3c, 0x3c, 0x3c)
    , m_fgFocused(0xb0, 0xb0, 0xb0)
    , m_bg(0xff, 0xff, 0xff)
    , m_font("monospace", 8)
    , m_hMargin(0)
    , m_vMargin(0)
    , m_hPad(4)
    , m_vPad(2)
    , m_textVertShift(1)
    , m_rounding(1)
    , m_indicatorWidth(0)
    , m_isFocused(false)
    , m_isChecked(false)
{
}

const QString& TagLabel::text() const
{
    return m_text;
}

void TagLabel::setText(QString text)
{
    if (m_text == text) {
        return;
    }
    m_text = text;
    updateSize();
    repaint();
}

void TagLabel::setCustomIndicator(QString indicator)
{
    if (m_customIndicator == indicator) {
        return;
    }
    m_customIndicator = indicator;
    updateSize();
    repaint();
}

void TagLabel::setFocused(bool focused)
{
    if (m_isFocused == focused) {
        return;
    }
    m_isFocused = focused;
    repaint();
}

void TagLabel::setChecked(bool checked)
{
    if (m_isChecked == checked) {
        return;
    }
    m_isChecked = checked;
    repaint();
}

void TagLabel::setColors(QColor background, QColor regular, QColor focused)
{
    if (m_bg == background && m_fgRegular == regular
        && m_fgFocused == focused) {
        return;
    }
    m_bg = background;
    m_fgRegular = regular;
    m_fgFocused = focused;
    repaint();
}

void TagLabel::setFont(const QFont& font)
{
    if (m_font == font) {
        return;
    }
    m_font = font;
    updateSize();
    repaint();
}

void TagLabel::setPadding(int h, int v)
{
    if (m_hPad == h && m_vPad == v) {
        return;
    }
    m_hPad = h;
    m_vPad = v;
    updateSize();
    repaint();
}

void TagLabel::setRounding(int r)
{
    if (m_rounding == r) {
        return;
    }
    m_rounding = r;
    repaint();
}

void TagLabel::updateSize()
{
    QFontMetrics metrics(m_font);

    m_indicatorWidth = metrics.width(m_customIndicator);

    int w = metrics.width(m_text) + m_indicatorWidth + m_hPad * 4;
    int h = metrics.height() + m_vPad * 2;

    m_rect.setWidth(w);
    m_rect.setHeight(h);

    setFixedWidth(w + 1);
    setFixedHeight(h + 1);
}

void TagLabel::paintEvent(QPaintEvent*)
{
    QPainter pt(this);

    const QRect mRect = QRect(m_rect.left() + m_hMargin, m_rect.top() + m_vMargin,
        m_rect.width() - 2 * m_hMargin, m_rect.height() - 2 * m_hMargin);

    pt.setRenderHint(QPainter::Qt4CompatiblePainting, true);

    if (m_isFocused) {
        pt.setPen(QPen(m_fgFocused, 1));
        pt.setBrush(QBrush(m_fgFocused));
    } else {
        pt.setPen(QPen(m_bg, 1));
        pt.setBrush(QBrush(m_bg));
    }

    pt.drawRoundedRect(mRect, m_rounding, m_rounding);

    if (m_isChecked && !m_isFocused) {
        pt.setPen(QPen(m_fgRegular, 1));
        pt.setBrush(QBrush(m_fgRegular));

        pt.drawRoundedRect(
            QRect(mRect.right() - m_indicatorWidth - m_hPad * 2, mRect.top(),
                m_indicatorWidth + m_hPad * 2, mRect.height()),
            m_rounding, m_rounding);
    } else {
        pt.setPen(QPen(m_fgRegular, 1));
        pt.drawLine(QLine(mRect.right() - m_indicatorWidth - m_hPad * 2, mRect.top(),
            mRect.right() - m_indicatorWidth - m_hPad * 2, mRect.bottom()));
    }

    pt.setPen(QPen(m_fgRegular, 1));
    pt.setBrush(QBrush());
    pt.drawRoundedRect(mRect, m_rounding, m_rounding);

    pt.setFont(m_font);

    if (m_isFocused) {
        pt.setPen(QPen(m_bg, 1));
    } else {
        pt.setPen(QPen(m_fgRegular, 1));
    }

    pt.drawText(QRect(mRect.left(), mRect.top() + m_textVertShift,
                    mRect.width() - m_indicatorWidth - m_hPad * 2, mRect.height()),
        Qt::AlignCenter, m_text);

    if (m_isChecked || m_isFocused) {
        pt.setPen(QPen(m_bg, 1));
    } else {
        pt.setPen(QPen(m_fgRegular, 1));
    }

    if (m_closeIndicator) {
        const auto centerX = mRect.right() - m_indicatorWidth / 2 - m_hPad;
        const auto centerY = mRect.top() + mRect.height() / 2;
        const auto half_size = mRect.height() / 3 / 2;

        pt.drawLine(QLine(centerX - half_size, centerY - half_size, centerX + half_size,
            centerY + half_size));

        pt.drawLine(QLine(centerX + half_size, centerY - half_size, centerX - half_size,
            centerY + half_size));
    } else {
        pt.drawText(QRect(mRect.right() - m_indicatorWidth - m_hPad * 2,
                        mRect.top() + m_textVertShift, m_indicatorWidth + m_hPad * 2,
                        mRect.height()),
            Qt::AlignCenter, m_customIndicator);
    }
}

void TagLabel::mousePressEvent(QMouseEvent*)
{
    clicked();
}

} // namespace tagberry::widgets
