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
#include <QMouseEvent>
#include <QPainter>

namespace tagberry::widgets {

TagLabel::TagLabel(QWidget* parent)
    : QWidget(parent)
    , m_closeButton(false)
    , m_fgRegular(0x3e, 0x3e, 0x3e)
    , m_fgFocused(0xb0, 0xb0, 0xb0)
    , m_bg(0xff, 0xff, 0xff)
    , m_font("monospace", 8)
    , m_hMargin(0)
    , m_vMargin(0)
    , m_hPad(5)
    , m_vPad(3)
    , m_textVertShift(1)
    , m_rounding(1)
    , m_indicatorWidth(0)
    , m_isFocused(false)
    , m_isChecked(false)
    , m_isClosePressed(false)
    , m_closePressStarted(false)
{
}

const QString& TagLabel::text() const
{
    return m_text;
}

void TagLabel::setClosable(bool v)
{
    if (m_closeButton == v) {
        return;
    }
    m_closeButton = v;
    updateSize();
    repaint();
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

void TagLabel::setColors(QColor regular, QColor focused)
{
    if (m_fgRegular == regular && m_fgFocused == focused) {
        return;
    }
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

void TagLabel::setMargin(int h, int v)
{
    if (m_hMargin == h && m_vMargin == v) {
        return;
    }
    m_hMargin = h;
    m_vMargin = v;
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

    auto s = m_customIndicator;
    if (s.isEmpty() || m_closeButton) {
        s = " ";
    }

    m_indicatorWidth = metrics.width(s);

    const int lw = metrics.width(m_text) + 2 * m_hPad + m_hMargin;
    const int rw = m_indicatorWidth + 2 * m_hPad + m_hMargin;

    const int w = lw + rw;
    const int h = metrics.height() + 2 * m_vPad + 2 * m_vMargin;

    m_innerRect.setLeft(m_hMargin);
    m_innerRect.setRight(w - 2 * m_hMargin);
    m_innerRect.setTop(m_vMargin);
    m_innerRect.setBottom(h - 2 * m_vMargin);

    m_closeRect.setLeft(lw);
    m_closeRect.setRight(w - m_hMargin);
    m_closeRect.setTop(m_vMargin);
    m_closeRect.setBottom(h - 2 * m_vMargin);

    setFixedWidth(w + 2);
    setFixedHeight(h + 2);
}

void TagLabel::paintEvent(QPaintEvent*)
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

    pt.drawRoundedRect(m_innerRect, m_rounding, m_rounding);

    if (m_isChecked && !m_isFocused) {
        if (m_isClosePressed) {
            pt.setPen(QPen(m_fgFocused, 1));
            pt.setBrush(QBrush(m_fgFocused));
        } else {
            pt.setPen(QPen(m_fgRegular, 1));
            pt.setBrush(QBrush(m_fgRegular));
        }

        pt.drawRoundedRect(
            QRect(m_innerRect.right() - m_indicatorWidth - m_hPad * 2, m_innerRect.top(),
                m_indicatorWidth + m_hPad * 2, m_innerRect.height()),
            m_rounding, m_rounding);
    }

    if (!m_isChecked || m_isFocused || m_isClosePressed) {
        pt.setPen(QPen(m_fgRegular, 1));

        pt.drawLine(QLine(m_innerRect.right() - m_indicatorWidth - m_hPad * 2,
            m_innerRect.top(), m_innerRect.right() - m_indicatorWidth - m_hPad * 2,
            m_innerRect.bottom()));
    }

    pt.setPen(QPen(m_fgRegular, 1));
    pt.setBrush(QBrush());
    pt.drawRoundedRect(m_innerRect, m_rounding, m_rounding);

    pt.setFont(m_font);

    if (m_isFocused) {
        pt.setPen(QPen(m_bg, 1));
    } else {
        pt.setPen(QPen(m_fgRegular, 1));
    }

    pt.drawText(
        QRect(m_innerRect.left(), m_innerRect.top() + m_textVertShift,
            m_innerRect.width() - m_indicatorWidth - m_hPad * 2, m_innerRect.height()),
        Qt::AlignCenter, m_text);

    if (m_isChecked || m_isFocused) {
        pt.setPen(QPen(m_bg, 1));
    } else {
        pt.setPen(QPen(m_fgRegular, 1));
    }

    if (m_closeButton) {
        const auto centerX = m_innerRect.right() - m_indicatorWidth / 2 - m_hPad;
        const auto centerY = m_innerRect.top() + m_innerRect.height() / 2;
        const auto half_size = m_innerRect.height() / 3 / 2;

        pt.drawLine(QLine(centerX - half_size, centerY - half_size, centerX + half_size,
            centerY + half_size));

        pt.drawLine(QLine(centerX + half_size, centerY - half_size, centerX - half_size,
            centerY + half_size));
    } else {
        pt.drawText(QRect(m_innerRect.right() - m_indicatorWidth - m_hPad * 2,
                        m_innerRect.top() + m_textVertShift,
                        m_indicatorWidth + m_hPad * 2, m_innerRect.height()),
            Qt::AlignCenter, m_customIndicator);
    }
}

void TagLabel::mousePressEvent(QMouseEvent* event)
{
    if (m_closeButton) {
        bool closePressed = m_closeRect.contains(event->pos());

        if (m_isClosePressed != closePressed) {
            m_closePressStarted = closePressed;
            m_isClosePressed = closePressed;
            repaint();
        }

        if (!closePressed) {
            clicked();
        }
    } else {
        clicked();
    }
}

void TagLabel::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_closeButton) {
        bool wasClosePressed = m_isClosePressed;

        m_closePressStarted = false;
        m_isClosePressed = false;

        repaint();

        if (wasClosePressed && m_closeRect.contains(event->pos())) {
            closeClicked();
        }
    }
}

void TagLabel::mouseMoveEvent(QMouseEvent* event)
{
    if (m_closePressStarted) {
        bool closePressed = m_closeRect.contains(event->pos());

        if (m_isClosePressed != closePressed) {
            m_isClosePressed = closePressed;
            repaint();
        }
    }
}

} // namespace tagberry::widgets
