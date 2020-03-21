/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/MarkdownEdit.hpp"

#include <QApplication>
#include <QFontMetrics>
#include <QDebug>

#include <qmarkdowntextedit.h>

namespace tagberry::widgets {

namespace {

QHash<MarkdownHighlighter::HighlighterState, QTextCharFormat> createFormatMap(
    const QHash<QString, QColor>& colorMap, int fontSize)
{
    QHash<MarkdownHighlighter::HighlighterState, QTextCharFormat> formatMap;

    // empty
    {
        auto format = QTextCharFormat();
        formatMap[MarkdownHighlighter::NoState] = format;
    }

    // headline end
    {
        auto format = QTextCharFormat();
        formatMap[MarkdownHighlighter::HeadlineEnd] = format;
    }

    // headlines
    {
        auto format = QTextCharFormat();
        format.setFontWeight(QFont::Bold);
        format.setForeground(QBrush(colorMap["text-light"]));

        format.setFontPointSize(fontSize * 1.4);
        formatMap[MarkdownHighlighter::H1] = format;

        format.setFontPointSize(fontSize * 1.3);
        formatMap[MarkdownHighlighter::H2] = format;

        format.setFontPointSize(fontSize * 1.2);
        formatMap[MarkdownHighlighter::H3] = format;

        format.setFontPointSize(fontSize * 1.1);
        formatMap[MarkdownHighlighter::H4] = format;

        format.setFontPointSize(fontSize * 1.0);
        formatMap[MarkdownHighlighter::H5] = format;

        format.setFontPointSize(fontSize * 1.0);
        formatMap[MarkdownHighlighter::H6] = format;
    }

    // horizontal ruler
    {
        auto format = QTextCharFormat();
        format.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
        format.setFontWeight(QFont::Bold);
        format.setForeground(QBrush(colorMap["text-extra-light"]));

        formatMap[MarkdownHighlighter::HorizontalRuler] = format;
    }

    // list
    {
        auto format = QTextCharFormat();
        format.setFontWeight(QFont::Bold);
        format.setForeground(QBrush(colorMap["text-extra-light"]));

        formatMap[MarkdownHighlighter::List] = format;
    }

    // link
    {
        auto format = QTextCharFormat();
        format.setFontUnderline(true);
        format.setForeground(QBrush(colorMap["text-url"]));

        formatMap[MarkdownHighlighter::Link] = format;
    }

    // image
    {
        auto format = QTextCharFormat();
        format.setFontWeight(QFont::Bold);
        format.setForeground(QBrush(colorMap["text-light"]));

        formatMap[MarkdownHighlighter::Image] = format;
    }

    // italic
    {
        auto format = QTextCharFormat();
        format.setFontWeight(QFont::StyleItalic);
        format.setFontItalic(true);

        formatMap[MarkdownHighlighter::Italic] = format;
    }

    // bold
    {
        auto format = QTextCharFormat();
        format.setFontWeight(QFont::Bold);
        format.setForeground(QBrush(colorMap["text-light"]));

        formatMap[MarkdownHighlighter::Bold] = format;
    }

    // comment
    {
        auto format = QTextCharFormat();
        format.setForeground(QBrush(colorMap["text-extra-light"]));

        formatMap[MarkdownHighlighter::Comment] = format;
    }

    // masked syntax
    {
        auto format = QTextCharFormat();
        format.setForeground(QBrush(colorMap["text-extra-light"]));

        formatMap[MarkdownHighlighter::MaskedSyntax] = format;
    }

    // table
    {
        auto format = QTextCharFormat();
        format.setFont(QFont("monospace", fontSize - 1));
        format.setForeground(QBrush(colorMap["text-light"]));

        formatMap[MarkdownHighlighter::Table] = format;
    }

    // block quote
    {
        auto format = QTextCharFormat();
        format.setFontWeight(QFont::Bold);
        format.setForeground(QBrush(colorMap["text-extra-light"]));

        formatMap[MarkdownHighlighter::BlockQuote] = format;
    }

    // code block
    {
        auto format = QTextCharFormat();
        format.setFont(QFont("monospace", fontSize - 1));
        format.setBackground(QBrush(colorMap["code-background"]));

        formatMap[MarkdownHighlighter::CodeBlock] = format;
        formatMap[MarkdownHighlighter::InlineCodeBlock] = format;
    }

    // code keyword
    {
        auto format = QTextCharFormat();
        format.setFont(QFont("monospace", fontSize - 1));
        format.setBackground(QBrush(colorMap["code-background"]));
        format.setForeground(QBrush(colorMap["code-keyword"]));

        formatMap[MarkdownHighlighter::CodeKeyWord] = format;
    }

    // code type
    {
        auto format = QTextCharFormat();
        format.setFont(QFont("monospace", fontSize - 1));
        format.setBackground(QBrush(colorMap["code-background"]));
        format.setForeground(QBrush(colorMap["code-type"]));

        formatMap[MarkdownHighlighter::CodeType] = format;
    }

    // code built-in
    {
        auto format = QTextCharFormat();
        format.setFont(QFont("monospace", fontSize - 1));
        format.setBackground(QBrush(colorMap["code-background"]));
        format.setForeground(QBrush(colorMap["code-builtin"]));

        formatMap[MarkdownHighlighter::CodeBuiltIn] = format;
    }

    // code string
    {
        auto format = QTextCharFormat();
        format.setFont(QFont("monospace", fontSize - 1));
        format.setBackground(QBrush(colorMap["code-background"]));
        format.setForeground(QBrush(colorMap["code-string"]));

        formatMap[MarkdownHighlighter::CodeString] = format;
    }

    // code number
    {
        auto format = QTextCharFormat();
        format.setFont(QFont("monospace", fontSize - 1));
        format.setBackground(QBrush(colorMap["code-background"]));
        format.setForeground(QBrush(colorMap["code-number"]));

        formatMap[MarkdownHighlighter::CodeNumLiteral] = format;
    }

    // code comment
    {
        auto format = QTextCharFormat();
        format.setFont(QFont("monospace", fontSize - 1));
        format.setBackground(QBrush(colorMap["code-background"]));
        format.setForeground(QBrush(colorMap["code-comment"]));

        formatMap[MarkdownHighlighter::CodeComment] = format;
    }

    // code other
    {
        auto format = QTextCharFormat();
        format.setFont(QFont("monospace", fontSize - 1));
        format.setBackground(QBrush(colorMap["code-background"]));
        format.setForeground(QBrush(colorMap["code-other"]));

        formatMap[MarkdownHighlighter::CodeOther] = format;
    }

    return formatMap;
}

} // namespace

MarkdownEdit::MarkdownEdit(QWidget* parent)
    : QWidget(parent)
    , m_edit(new QMarkdownTextEdit)
{
    setLayout(&m_layout);

    m_layout.setContentsMargins(QMargins(m_hMargin, m_vMargin, m_hMargin, m_vMargin));
    m_layout.addWidget(m_edit);

    m_edit->setFrameStyle(QFrame::NoFrame);
    m_edit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_edit->installEventFilter(this);

    auto font = m_edit->font();
    font.setPointSize(m_fontSize);
    m_edit->setFont(font);

    connect(m_edit, &QMarkdownTextEdit::textChanged, this, &MarkdownEdit::updateText);
    connect(qApp, &QApplication::focusChanged, this, &MarkdownEdit::catchFocus);

    setColors(QHash<QString, QColor> {});
    updateText();
}

QString MarkdownEdit::text() const
{
    return m_edit->toPlainText();
}

void MarkdownEdit::setText(const QString& str)
{
    if (text() == str) {
        return;
    }
    m_edit->setText(str);
}

void MarkdownEdit::setColors(const QHash<QString, QColor>& colors)
{
    m_edit->highlighter()->setTextFormats(createFormatMap(colors, m_fontSize));
    m_edit->setText(text());
}

bool MarkdownEdit::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == m_edit) {
        if (event->type() == QEvent::KeyPress) {
            auto keyEvent = static_cast<QKeyEvent*>(event);

            if ((keyEvent->key() == Qt::Key_F)
                && keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
                return true;
            }

            if ((keyEvent->key() == Qt::Key_R)
                && keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
                return true;
            }
        }
    }
    return false;
}

void MarkdownEdit::catchFocus(QWidget* old, QWidget* now)
{
    if (now == m_edit) {
        clicked();
        m_edit->setFocus(Qt::MouseFocusReason);
        return;
    }

    if (old == m_edit) {
        auto curText = text();
        if (m_lastText != curText) {
            m_lastText = curText;
            editingFinished(curText);
        }
        auto cursor = m_edit->textCursor();
        cursor.clearSelection();
        m_edit->setTextCursor(cursor);
    }
}

void MarkdownEdit::updateText()
{
    auto doc = m_edit->document();
    auto layout = doc->documentLayout();

    qreal docHeight {};
    for (auto block = doc->begin(); block != doc->end(); block = block.next()) {
        docHeight += layout->blockBoundingRect(block).height();
    }

    auto widgetHeight = int(docHeight + doc->documentMargin());

    m_edit->setFixedHeight(widgetHeight + 2);
    setFixedHeight(widgetHeight + m_vMargin * 2 + 1);

    textChanged(text());
}

} // namespace tagberry::widgets
