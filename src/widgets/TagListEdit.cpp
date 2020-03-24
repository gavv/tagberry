/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/TagListEdit.hpp"

#include <QApplication>
#include <QCommonStyle>
#include <QResizeEvent>

namespace tagberry::widgets {

TagListEdit::TagListEdit(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_layout.addWidget(&m_addTagButton);

    setLayout(&m_layout);

    m_addTagButton.installEventFilter(this);
    m_addTagButton.setFixedSize(28, 28);
    m_addTagButton.setIcon(QIcon(":/icons/plus.png"));

    connect(&m_addTagButton, &QPushButton::clicked, this, &TagListEdit::handleTagAdd);

    connect(qApp, &QApplication::focusChanged, this, &TagListEdit::handleFocusChange);
}

QList<TagLabel*> TagListEdit::tags() const
{
    return m_tags;
}

void TagListEdit::setTags(QList<TagLabel*> tags)
{
    while (m_tags.count() != 0) {
        detachTag(m_tags[0]);
    }

    for (auto tag : tags) {
        attachTag(tag);
    }
}

void TagListEdit::attachTag(TagLabel* tag)
{
    tag->setMargin(1, 2);
    tag->setClosable(true);
    tag->setComplete(true);
    tag->setEditable(true);

    m_layout.removeWidget(&m_addTagButton);
    m_layout.addWidget(tag);
    m_layout.addWidget(&m_addTagButton);

    connect(tag, &TagLabel::closeClicked, this, &TagListEdit::handleTagRemove);
    connect(tag, &TagLabel::clicked, this, &TagListEdit::handleTagClick);
    connect(tag, &TagLabel::editingStarted, this, &TagListEdit::tagFocusCleared);
    connect(tag, &TagLabel::editingFinished, this, &TagListEdit::handleTagUpdate);

    m_tags.append(tag);
}

void TagListEdit::detachTag(TagLabel* tag)
{
    m_layout.removeWidget(tag);

    m_tags.removeAll(tag);
    tag->deleteLater();
}

void TagListEdit::removeEmptyTags()
{
    bool removed = false;

    for (auto it = m_tags.begin(); it != m_tags.end();) {
        auto tag = *it;
        if (!tag->editingNow() && tag->text().isEmpty()) {
            it = m_tags.erase(it);
            m_layout.removeWidget(tag);
            tag->deleteLater();
            removed = true;
        } else {
            ++it;
        }
    }

    if (removed) {
        tagsChanged();
    }
}

void TagListEdit::handleTagAdd()
{
    clicked();

    removeEmptyTags();

    auto tag = new TagLabel;

    attachTag(tag);
    tagAdded(tag);

    tag->startEditing();
}

void TagListEdit::handleTagRemove()
{
    auto tag = qobject_cast<TagLabel*>(sender());

    detachTag(tag);

    tagsChanged();
    clicked();
}

void TagListEdit::handleTagUpdate(QString oldText, QString)
{
    auto editedTag = qobject_cast<TagLabel*>(sender());

    if (editedTag->text().isEmpty()) {
        if (qApp->focusWidget() == &m_addTagButton) {
            return;
        }
        m_layout.removeWidget(editedTag);
        m_tags.removeAll(editedTag);
        editedTag->deleteLater();
        if (oldText.isEmpty()) {
            return;
        }
    } else {
        for (auto it = m_tags.begin(); it != m_tags.end();) {
            auto oldTag = *it;
            if (oldTag->text() == editedTag->text() && oldTag != editedTag) {
                it = m_tags.erase(it);
                m_layout.removeWidget(oldTag);
                oldTag->deleteLater();
            } else {
                ++it;
            }
        }
    }

    tagsChanged();
}

void TagListEdit::handleTagClick()
{
    auto tag = qobject_cast<TagLabel*>(sender());

    tagFocusChanged(tag);
    clicked();
}

void TagListEdit::handleFocusChange(QWidget*, QWidget* now)
{
    if (now == &m_addTagButton) {
        now->setFocus(Qt::MouseFocusReason);
    }
}

void TagListEdit::resizeEvent(QResizeEvent* event)
{
    m_layout.setPreferredWidth(event->size().width());
}

bool TagListEdit::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == &m_addTagButton) {
        switch (int(event->type())) {
        case QEvent::MouseButtonRelease:
        case QEvent::Leave:
        case QEvent::HoverLeave:
        case QEvent::WindowDeactivate:
        case QEvent::FocusOut:
        case QEvent::Hide:
            removeEmptyTags();
            break;
        }
    }
    return false;
}

} // namespace tagberry::widgets
