/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/RecordCell.hpp"

#include <QApplication>
#include <QCommonStyle>

namespace tagberry::widgets {

RecordCell::RecordCell(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout.addWidget(&m_cell);

    setLayout(&m_layout);

    m_complete.setFixedWidth(24);
    updateStyleSheet("#ffffff", "#000000");

    m_headerLayout.setContentsMargins(QMargins(3, 3, 3, 3));
    m_headerLayout.setSpacing(4);
    m_headerLayout.addWidget(&m_complete);
    m_headerLayout.addWidget(&m_title);
    m_headerLayout.setAlignment(&m_complete, Qt::AlignTop);

    m_bodyLayout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_bodyLayout.addWidget(&m_tagListEdit);

    m_cell.setRowLayout(Row_Title, &m_headerLayout, 0);
    m_cell.setRowLayout(Row_Tags, &m_bodyLayout, 1);

    connect(&m_cell, &Cell::clicked, this, &RecordCell::cellClicked);

    m_title.setPlaceholderText("enter title");
    m_title.setAcceptRichText(false);
    m_title.setFrameStyle(QFrame::NoFrame);
    m_title.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_title.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto font = m_title.font();
    font.setPointSize(10);
    font.setWeight(QFont::Bold);
    m_title.setFont(font);

    connect(&m_complete, &QCheckBox::stateChanged, this,
        [=] { completeChanged(complete()); });

    connect(&m_title, &QTextEdit::textChanged, this, &RecordCell::updateTitle);

    connect(&m_tagListEdit, &TagListEdit::clicked, this, [=] { clicked(this); });

    connect(&m_tagListEdit, &TagListEdit::tagsChanged, this, &RecordCell::tagsChanged);
    connect(&m_tagListEdit, &TagListEdit::tagAdded, this, &RecordCell::tagAdded);
    connect(&m_tagListEdit, &TagListEdit::tagFocusChanged, this,
        &RecordCell::tagFocusChanged);
    connect(&m_tagListEdit, &TagListEdit::tagFocusCleared, this,
        &RecordCell::tagFocusCleared);

    connect(qApp, &QApplication::focusChanged, this, &RecordCell::catchFocus);
}

void RecordCell::setTags(QList<TagLabel*> tags)
{
    m_tagListEdit.setTags(tags);
}

void RecordCell::setComplete(bool complete)
{
    m_complete.setChecked(complete);
}

void RecordCell::setTitle(QString str)
{
    if (title() == str) {
        return;
    }
    m_title.setText(str);
}

void RecordCell::setFocused(bool focused)
{
    m_cell.setFocused(focused);
    if (!focused) {
        auto cursor = m_title.textCursor();
        cursor.clearSelection();
        m_title.setTextCursor(cursor);
    }
}

void RecordCell::startEditing()
{
    m_title.setFocus(Qt::MouseFocusReason);
}

bool RecordCell::complete() const
{
    return m_complete.isChecked();
}

QString RecordCell::title() const
{
    return m_title.toPlainText();
}

QList<TagLabel*> RecordCell::tags() const
{
    return m_tagListEdit.tags();
}

void RecordCell::notifyRemoving()
{
    removing();
}

void RecordCell::cellClicked()
{
    tagFocusCleared();
    clicked(this);
}

void RecordCell::updateTitle()
{
    auto str = title();

    if (str.contains('\r') || str.contains('\n')) {
        auto cursor = m_title.textCursor();
        str.remove('\r');
        str.remove('\n');
        m_title.setText(str);
        m_title.setTextCursor(cursor);
        return;
    }

    auto size = m_title.document()->size().toSize().height() + 2;

    m_title.setFixedHeight(size);

    titleChanged(title());
}

void RecordCell::catchFocus(QWidget* old, QWidget* now)
{
    if (now == &m_complete || now == &m_title) {
        clicked(this);
        now->setFocus(Qt::MouseFocusReason);
        return;
    }

    if (old == &m_title) {
        auto curTitle = title();
        if (m_lastTitle != curTitle) {
            m_lastTitle = curTitle;
            titleEditingFinished(curTitle);
        }
        auto cursor = m_title.textCursor();
        cursor.clearSelection();
        m_title.setTextCursor(cursor);
    }
}

void RecordCell::paintEvent(QPaintEvent* event)
{
    if (m_firstPaint) {
        m_firstPaint = false;
        updateTitle();
    }
    QWidget::paintEvent(event);
}

void RecordCell::setColors(QHash<QString, QColor> colors)
{
    m_cell.setBorderColor(colors["border"]);
    m_cell.setRowColor(Row_Title, colors["background"]);
    m_cell.setRowColor(Row_Tags, colors["background"]);
    updateStyleSheet(colors["background-dimmed"], colors["border"]);
}

void RecordCell::updateStyleSheet(QColor bg, QColor border)
{
    auto style = R"(
      QCheckBox::indicator {
        width: 17px;
        height: 17px;
        margin: 4px;
        border-style: solid;
        border-width: 1px;
        border-radius: 2px;
        border-color: %1;
      }
      QCheckBox::indicator:unchecked {
        image: none;
      }
      QCheckBox::indicator:checked {
        image: url(:/icons/mark.png);
        background-color: %2;
      }
)";
    m_complete.setStyleSheet(QString(style).arg(border.name(), bg.name()));
}

} // namespace tagberry::widgets
