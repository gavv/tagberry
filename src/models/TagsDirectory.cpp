/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "models/TagsDirectory.hpp"

namespace tagberry::models {

TagsDirectory::TagsDirectory(QObject* parent)
    : QObject(parent)
{
}

QList<Tag*> TagsDirectory::getTags() const
{
    return m_tags.values();
}

Tag* TagsDirectory::getTag(const QString& name) const
{
    return m_tags[name];
}

Tag* TagsDirectory::getOrCreateTag(const QString& name)
{
    if (auto tag = m_tags[name]) {
        return tag;
    }

    auto tag = new Tag(this);

    tag->setName(name);

    connect(tag, &Tag::nameChanged, this, &TagsDirectory::tagTextChanged);
    connect(tag, &Tag::focusChanged, this, &TagsDirectory::tagFocusChanged);

    m_tags[tag->name()] = tag;

    return tag;
}

void TagsDirectory::removeTag(Tag* tag)
{
    m_tags.remove(tag->name());

    delete tag;
}

void TagsDirectory::focusTag(Tag* focusedTag)
{
    for (auto tag : m_tags) {
        tag->setFocused(tag == focusedTag);
    }
}

void TagsDirectory::clearTags()
{
    for (auto it = m_tags.begin(); it != m_tags.end(); it = m_tags.erase(it)) {
        delete it.value();
    }
}

void TagsDirectory::tagTextChanged(QString text)
{
    auto tag = qobject_cast<Tag*>(sender());

    for (auto it = m_tags.begin(); it != m_tags.end(); ++it) {
        if (it.value() == tag) {
            m_tags.erase(it);
            break;
        }
    }

    m_tags[text] = tag;
}

void TagsDirectory::tagFocusChanged(bool focused)
{
    auto tag = qobject_cast<Tag*>(sender());

    if (focused) {
        focusTag(tag);
    } else {
        focusTag(nullptr);
    }
}

} // tagberry::models
