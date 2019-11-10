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
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout.addWidget(&m_cell);

    setLayout(&m_layout);

    m_headLayout.setContentsMargins(QMargins(3, 3, 3, 3));
    m_headLayout.addWidget(&m_title);

    m_bodyLayout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_bodyLayout.addWidget(&m_tagSelector);

    m_cell.setHeadLayout(&m_headLayout);
    m_cell.setBodyLayout(&m_bodyLayout);

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

    connect(&m_title, &QTextEdit::textChanged, this, &RecordCell::updateTitle);

    connect(&m_tagSelector, &TagSelector::clicked, this, [=] { clicked(this); });

    connect(&m_tagSelector, &TagSelector::tagsChanged, this, &RecordCell::tagsChanged);
    connect(&m_tagSelector, &TagSelector::tagAdded, this, &RecordCell::tagAdded);
    connect(&m_tagSelector, &TagSelector::tagFocusChanged, this,
        &RecordCell::tagFocusChanged);
    connect(&m_tagSelector, &TagSelector::tagFocusCleared, this,
        &RecordCell::tagFocusCleared);

    connect(qApp, &QApplication::focusChanged, this, &RecordCell::catchFocus);
}

void RecordCell::setTags(QList<TagLabel*> tags)
{
    m_tagSelector.setTags(tags);
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
}

void RecordCell::startEditing()
{
    m_title.setFocus(Qt::MouseFocusReason);
}

QString RecordCell::title() const
{
    return m_title.toPlainText();
}

QList<TagLabel*> RecordCell::tags() const
{
    return m_tagSelector.tags();
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
    if (now == &m_title) {
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
    }
}

void RecordCell::showEvent(QShowEvent* event)
{
    if (m_firstShow) {
        m_firstShow = false;
        updateTitle();
    }
    QWidget::showEvent(event);
}

} // namespace tagberry::widgets
