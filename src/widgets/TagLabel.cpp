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

namespace {

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
int horizontalAdvance(const QFontMetrics& m, const QString& s)
{
    return m.horizontalAdvance(s);
}
#else
int horizontalAdvance(const QFontMetrics& m, const QString& s)
{
    return m.width(s);
}
#endif

} // namespace

TagLabel::TagLabel(QWidget* parent)
    : QWidget(parent)
    , m_edit(nullptr)
    , m_closeButton(false)
    , m_isEditable(false)
    , m_fgRegular(0x3e, 0x3e, 0x3e)
    , m_fgFocusedComplete(0xb0, 0xb0, 0xb0)
    , m_fgFocusedIncomplete(0xe0, 0xe0, 0xe0)
    , m_bg(0xff, 0xff, 0xff)
    , m_font("monospace", 8)
    , m_hMargin(0)
    , m_vMargin(0)
    , m_hPad(5)
    , m_vPad(3)
    , m_textVertShift(1)
    , m_rounding(1)
    , m_cursorWidth(2)
    , m_indicatorWidth(0)
    , m_isFocused(false)
    , m_isComplete(false)
    , m_isClosePressed(false)
    , m_closePressStarted(false)
    , m_cacheValid(false)
{
    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));

    setLayout(&m_layout);
}

bool TagLabel::editingNow() const
{
    return m_edit;
}

const QString& TagLabel::text() const
{
    return m_text;
}

bool TagLabel::isFocused() const
{
    return m_isFocused;
}

void TagLabel::setClosable(bool v)
{
    if (m_closeButton == v) {
        return;
    }
    m_closeButton = v;
    m_cacheValid = false;
    updateSizes();
    update();
}

void TagLabel::setEditable(bool v)
{
    if (m_isEditable == v) {
        return;
    }
    m_isEditable = v;
    m_cacheValid = false;
    updateSizes();
    update();
}

void TagLabel::setText(QString text)
{
    if (m_text == text) {
        return;
    }
    m_text = text;
    m_cacheValid = false;
    updateSizes();
    update();
    textChanged(text);
}

void TagLabel::setCustomIndicator(QString indicator)
{
    if (m_customIndicator == indicator) {
        return;
    }
    m_customIndicator = indicator;
    m_cacheValid = false;
    updateSizes();
    update();
}

void TagLabel::setFocused(bool focused)
{
    if (m_isFocused == focused) {
        return;
    }
    m_isFocused = focused;
    m_cacheValid = false;
    updateColors();
    update();
}

void TagLabel::setComplete(bool checked)
{
    if (m_isComplete == checked) {
        return;
    }
    m_isComplete = checked;
    m_cacheValid = false;
    update();
}

void TagLabel::setColors(QHash<QString, QColor> colors)
{
    auto bg = colors["background"];
    auto regular = colors["regular"];
    auto focusedComplete = colors["focused-complete"];
    auto focusedIncomplete = colors["focused-incomplete"];

    if (m_bg == bg && m_fgRegular == regular && m_fgFocusedComplete == focusedComplete
        && m_fgFocusedIncomplete == focusedIncomplete) {
        return;
    }

    m_bg = bg;
    m_fgRegular = regular;
    m_fgFocusedComplete = focusedComplete;
    m_fgFocusedIncomplete = focusedIncomplete;
    m_cacheValid = false;

    updateColors();
    update();
}

void TagLabel::setFont(const QFont& font)
{
    if (m_font == font) {
        return;
    }
    m_font = font;
    m_cacheValid = false;
    if (m_edit) {
        m_edit->setFont(font);
    }
    updateSizes();
    update();
}

void TagLabel::setPadding(int h, int v)
{
    if (m_hPad == h && m_vPad == v) {
        return;
    }
    m_hPad = h;
    m_vPad = v;
    m_cacheValid = false;
    updateSizes();
    update();
}

void TagLabel::setMargin(int h, int v)
{
    if (m_hMargin == h && m_vMargin == v) {
        return;
    }
    m_hMargin = h;
    m_vMargin = v;
    m_cacheValid = false;
    updateSizes();
    update();
}

void TagLabel::setRounding(int r)
{
    if (m_rounding == r) {
        return;
    }
    m_rounding = r;
    m_cacheValid = false;
    update();
}

void TagLabel::updateSizes()
{
    QFontMetrics metrics(m_font);

    auto s = m_customIndicator;
    if (s.isEmpty() || m_closeButton) {
        s = " ";
    }

    m_indicatorWidth = horizontalAdvance(metrics, s);

    const int lw
        = horizontalAdvance(metrics, m_text) + 2 * m_hPad + m_hMargin + m_cursorWidth;
    const int rw = m_indicatorWidth + 2 * m_hPad + m_hMargin;

    const int w = lw + rw;
    const int h = metrics.height() + 2 * m_vPad + 2 * m_vMargin;

    m_innerRect.setLeft(m_hMargin);
    m_innerRect.setRight(w - m_hMargin);
    m_innerRect.setTop(m_vMargin);
    m_innerRect.setBottom(h - m_vMargin);

    m_closeRect.setLeft(lw);
    m_closeRect.setRight(w - m_hMargin);
    m_closeRect.setTop(m_vMargin);
    m_closeRect.setBottom(h - m_vMargin);

    setFixedWidth(w + 2);
    setFixedHeight(h + 2);

    m_layout.setContentsMargins(QMargins(m_hMargin + m_hPad - 1,
        m_vMargin + m_vPad + m_textVertShift, rw + m_hPad, m_vMargin + m_vPad));
}

void TagLabel::updateColors()
{
    if (m_edit) {
        QPalette palette;
        if (m_isFocused) {
            palette.setColor(QPalette::Text, m_bg);
        } else {
            palette.setColor(QPalette::Text, m_fgRegular);
        }
        m_edit->setPalette(palette);
    }
}

void TagLabel::paintEvent(QPaintEvent*)
{
    if (m_edit) {
        QPainter pt(this);
        doPaint(pt);
        return;
    }

    if (!m_cacheValid) {
        m_cache = QPixmap(size());
        m_cacheValid = true;

        QPainter pt(&m_cache);
        doPaint(pt);
    }

    QPainter pt(this);
    pt.drawPixmap(0, 0, m_cache);
}

void TagLabel::doPaint(QPainter& pt)
{
    pt.setRenderHint(QPainter::Qt4CompatiblePainting, true);

    pt.setPen(QPen(m_bg, 1));
    pt.setBrush(QBrush(m_bg));
    pt.drawRect(QRect(QPoint(0, 0), size()));

    if (m_isFocused && m_isComplete) {
        pt.setPen(QPen(m_fgFocusedComplete, 1));
        pt.setBrush(QBrush(m_fgFocusedComplete));
    } else if (m_isFocused) {
        pt.setPen(QPen(m_fgFocusedIncomplete, 1));
        pt.setBrush(QBrush(m_fgFocusedIncomplete));
    } else {
        pt.setPen(QPen(m_bg, 1));
        pt.setBrush(QBrush(m_bg));
    }

    pt.drawRoundedRect(m_innerRect, m_rounding, m_rounding);

    if (m_isComplete && !m_isFocused) {
        if (m_isClosePressed) {
            pt.setPen(QPen(m_fgFocusedComplete, 1));
            pt.setBrush(QBrush(m_fgFocusedComplete));
        } else {
            pt.setPen(QPen(m_fgRegular, 1));
            pt.setBrush(QBrush(m_fgRegular));
        }

        pt.drawRoundedRect(
            QRect(m_innerRect.right() - m_indicatorWidth - m_hPad * 2, m_innerRect.top(),
                m_indicatorWidth + m_hPad * 2, m_innerRect.height()),
            m_rounding, m_rounding);
    }

    if (!m_isComplete || m_isFocused || m_isClosePressed) {
        pt.setPen(QPen(m_fgRegular, 1));

        pt.drawLine(QLine(m_innerRect.right() - m_indicatorWidth - m_hPad * 2,
            m_innerRect.top(), m_innerRect.right() - m_indicatorWidth - m_hPad * 2,
            m_innerRect.bottom()));
    }

    pt.setPen(QPen(m_fgRegular, 1));
    pt.setBrush(QBrush());
    pt.drawRoundedRect(m_innerRect, m_rounding, m_rounding);

    pt.setFont(m_font);

    if (m_isFocused && m_isComplete) {
        pt.setPen(QPen(m_bg, 1));
    } else {
        pt.setPen(QPen(m_fgRegular, 1));
    }

    if (!m_edit) {
        pt.drawText(QRect(m_innerRect.left(), m_innerRect.top() + m_textVertShift,
                        m_innerRect.width() - m_indicatorWidth - m_hPad * 2,
                        m_innerRect.height()),
            Qt::AlignCenter, m_text);
    }

    if (m_isComplete) {
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
    if (event->button() != Qt::LeftButton) {
        return;
    }

    if (m_closeButton) {
        bool closePressed = m_closeRect.contains(event->pos());

        if (m_isClosePressed != closePressed) {
            m_closePressStarted = closePressed;
            m_isClosePressed = closePressed;
            m_cacheValid = false;
            update();
        }

        if (!closePressed && !m_edit) {
            clicked();
        }
    } else if (!m_edit) {
        clicked();
    }
}

void TagLabel::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    if (m_closeButton) {
        bool wasClosePressed = m_isClosePressed;

        m_closePressStarted = false;
        m_isClosePressed = false;
        m_cacheValid = false;

        update();

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
            m_cacheValid = false;
            update();
        }
    }
}

void TagLabel::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    if (m_closeRect.contains(event->pos())) {
        return;
    }

    startEditing();
}

void TagLabel::startEditing()
{
    if (!m_isEditable || m_edit) {
        return;
    }

    setFocused(false);
    editingStarted();

    m_edit = new QLineEdit;

    m_edit->setText(m_text);
    m_edit->setFrame(false);
    m_edit->setFont(m_font);
    m_edit->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); }");

    updateColors();

    m_oldText = m_text;

    connect(m_edit, &QLineEdit::textChanged, this, &TagLabel::setText);
    connect(m_edit, &QLineEdit::editingFinished, this, &TagLabel::finishEditing);

    m_layout.addWidget(m_edit);

    m_edit->setFocus(Qt::MouseFocusReason);
}

void TagLabel::finishEditing()
{
    if (!m_edit) {
        return;
    }

    setText(m_edit->text());

    m_edit->deleteLater();
    m_edit = nullptr;

    editingFinished(m_oldText, m_text);

    m_oldText.clear();
}

} // namespace tagberry::widgets
