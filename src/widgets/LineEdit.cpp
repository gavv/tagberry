/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/LineEdit.hpp"

#include <QApplication>
#include <QCommonStyle>

namespace tagberry::widgets {

LineEdit::LineEdit(QWidget* parent)
    : QWidget(parent)
{
    setLayout(&m_layout);

    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout.addWidget(&m_edit);

    m_edit.setAcceptRichText(false);
    m_edit.setFrameStyle(QFrame::NoFrame);
    m_edit.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_edit.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_edit.installEventFilter(this);

    auto f = m_edit.font();
    f.setPointSize(10);
    m_edit.setFont(f);

    connect(&m_edit, &QTextEdit::textChanged, this, &LineEdit::updateText);
    connect(qApp, &QApplication::focusChanged, this, &LineEdit::catchFocus);
}

QString LineEdit::text() const
{
    return m_edit.toPlainText();
}

void LineEdit::setText(const QString& str)
{
    if (text() == str) {
        return;
    }
    m_edit.setText(str);
}

void LineEdit::setPlaceholderText(const QString& str)
{
    m_edit.setPlaceholderText(str);
}

void LineEdit::setFontWeight(QFont::Weight weight)
{
    auto f = m_edit.font();
    f.setWeight(weight);
    m_edit.setFont(f);
}

void LineEdit::setFocused(bool focused)
{
    if (!focused) {
        auto cursor = m_edit.textCursor();
        cursor.clearSelection();
        m_edit.setTextCursor(cursor);
    }
}

void LineEdit::startEditing()
{
    m_edit.setFocus(Qt::MouseFocusReason);
}

void LineEdit::updateText()
{
    auto str = text();

    if (str.contains('\r') || str.contains('\n')) {
        auto cursor = m_edit.textCursor();
        str.remove('\r');
        str.remove('\n');
        m_edit.setText(str);
        m_edit.setTextCursor(cursor);
        return;
    }

    auto size = m_edit.document()->size().toSize().height() + 2;
    m_edit.setFixedHeight(size);

    textChanged(text());
}

void LineEdit::catchFocus(QWidget* old, QWidget* now)
{
    if (now == &m_edit) {
        clicked();
        m_edit.setFocus(Qt::MouseFocusReason);
        return;
    }

    if (old == &m_edit) {
        auto curText = text();
        if (m_lastText != curText) {
            m_lastText = curText;
            editingFinished(curText);
        }
        auto cursor = m_edit.textCursor();
        cursor.clearSelection();
        m_edit.setTextCursor(cursor);
    }
}

bool LineEdit::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == &m_edit) {
        if (event->type() == QEvent::Resize) {
            m_firstPaint = true;
        }
    }
    return false;
}

void LineEdit::paintEvent(QPaintEvent* event)
{
    if (m_firstPaint) {
        m_firstPaint = false;
        updateText();
    }
    QWidget::paintEvent(event);
}

} // namespace tagberry::widgets
