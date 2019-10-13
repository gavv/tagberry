/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/TagSelector.hpp"

#include <QApplication>
#include <QCommonStyle>

namespace tagberry::widgets {

TagSelector::TagSelector(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_layout.addWidget(&m_addTagButton);

    setLayout(&m_layout);

    m_addTagButton.setFixedSize(28, 28);
    m_addTagButton.setIcon(QIcon(":/icons/plus.png"));

    connect(&m_addTagButton, &QPushButton::clicked, this, &TagSelector::addTag);

    connect(qApp, &QApplication::focusChanged, this, &TagSelector::catchFocus);
}

QList<TagLabel*> TagSelector::tags() const
{
    return m_tags;
}

void TagSelector::addTag()
{
    auto tag = new TagLabel;

    tag->setMargin(1, 2);
    tag->setClosable(true);
    tag->setChecked(true);
    tag->setEditable(true);

    m_layout.removeWidget(&m_addTagButton);
    m_layout.addWidget(tag);
    m_layout.addWidget(&m_addTagButton);

    connect(tag, &TagLabel::closeClicked, this, &TagSelector::removeTag);
    connect(tag, &TagLabel::clicked, this, &TagSelector::catchTagClick);
    connect(tag, &TagLabel::editingStarted, this, &TagSelector::tagFocusCleared);

    m_tags.append(tag);

    tagAdded(tag);
    tagsChanged();

    tag->startEditing();
}

void TagSelector::removeTag()
{
    auto tag = qobject_cast<TagLabel*>(sender());

    m_layout.removeWidget(tag);

    m_tags.removeAll(tag);
    tag->deleteLater();

    tagsChanged();
    clicked();
}

void TagSelector::catchTagClick()
{
    auto tag = qobject_cast<TagLabel*>(sender());

    tagFocusChanged(tag);
    clicked();
}

void TagSelector::catchFocus(QWidget*, QWidget* now)
{
    if (now == &m_addTagButton) {
        clicked();
        now->setFocus(Qt::MouseFocusReason);
    }
}

} // namespace tagberry::widgets
