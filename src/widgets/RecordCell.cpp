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

    m_cell.setHeadLayout(&m_headLayout);
    m_cell.setBodyLayout(&m_bodyLayout);

    connect(&m_cell, &Cell::clicked, this, &RecordCell::cellClicked);

    m_title.setPlaceholderText("enter title");
    m_title.setAcceptRichText(false);
    m_title.setFrameStyle(QFrame::NoFrame);
    m_title.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_title.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto font = m_title.font();
    font.setPointSize(11);
    m_title.setFont(font);

    connect(&m_title, &QTextEdit::textChanged, this, &RecordCell::updateTitle);

    m_addTag.setFixedSize(28, 28);
    m_addTag.setIcon(QIcon(":/icons/plus.png"));

    m_bodyLayout.addWidget(&m_addTag);

    connect(&m_addTag, &QPushButton::clicked, this, &RecordCell::addTag);

    connect(qApp, &QApplication::focusChanged, this, &RecordCell::catchFocus);
}

void RecordCell::setFocused(bool focused)
{
    m_cell.setFocused(focused);
}

QString RecordCell::title() const
{
    return m_title.toPlainText();
}

QList<TagLabel*> RecordCell::tags() const
{
    return m_tags;
}

void RecordCell::addTag()
{
    auto tag = new TagLabel;

    tag->setMargin(1, 2);
    tag->setClosable(true);
    tag->setChecked(true);
    tag->setEditable(true);

    m_bodyLayout.removeWidget(&m_addTag);
    m_bodyLayout.addWidget(tag);
    m_bodyLayout.addWidget(&m_addTag);

    connect(tag, &TagLabel::closeClicked, this, &RecordCell::removeTag);
    connect(tag, &TagLabel::clicked, this, &RecordCell::tagClicked);
    connect(tag, &TagLabel::editingStarted, this, &RecordCell::tagFocusCleared);

    m_tags.append(tag);

    tagAdded(tag);
    tagsChanged();

    tag->startEditing();
}

void RecordCell::removeTag()
{
    auto tag = qobject_cast<TagLabel*>(sender());

    m_bodyLayout.removeWidget(tag);

    m_tags.removeAll(tag);
    delete tag;

    tagsChanged();
    clicked(this);
}

void RecordCell::cellClicked()
{
    tagFocusCleared();
    clicked(this);
}

void RecordCell::tagClicked()
{
    auto tag = qobject_cast<TagLabel*>(sender());

    tagFocusChanged(tag);
    clicked(this);
}

void RecordCell::updateTitle()
{
    auto size = m_title.document()->size().toSize().height() + 2;

    m_title.setFixedHeight(size);

    titleChanged(title());
}

void RecordCell::catchFocus(QWidget*, QWidget* now)
{
    if (now == &m_title || now == &m_addTag) {
        clicked(this);
        now->setFocus(Qt::MouseFocusReason);
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
