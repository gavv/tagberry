/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "models/TagsDirectory.hpp"

namespace tagberry::models {

QList<TagPtr> TagsDirectory::getTags() const
{
    return m_tagByID.values();
}

TagPtr TagsDirectory::getTagByName(const QString& name) const
{
    return m_tagByName[name];
}

TagPtr TagsDirectory::createTag(const QString& id)
{
    auto tag = std::make_shared<Tag>(id);

    if (m_colorScheme) {
        tag->setColorScheme(m_colorScheme);
    }

    connect(tag.get(), &Tag::nameChanged, this, &TagsDirectory::tagNameChanged);
    connect(tag.get(), &Tag::focusChanged, this, &TagsDirectory::tagFocusChanged);

    m_tagByID[tag->id()] = tag;

    return tag;
}

TagPtr TagsDirectory::getOrCreateTag(const QString& id)
{
    if (auto tag = m_tagByID[id]) {
        return tag;
    }

    return createTag(id);
}

void TagsDirectory::removeTag(TagPtr tag)
{
    disconnect(tag.get(), nullptr, this, nullptr);

    m_tagByID.remove(tag->id());

    if (!tag->name().isEmpty()) {
        m_tagByName.remove(tag->name());
    }
}

void TagsDirectory::focusTag(TagPtr focusedTag)
{
    for (auto tag : m_tagByID) {
        tag->setFocused(tag == focusedTag);
    }
}

void TagsDirectory::clearTags()
{
    for (auto tag : m_tagByID) {
        disconnect(tag.get(), nullptr, this, nullptr);
    }

    m_tagByName.clear();
    m_tagByID.clear();
}

void TagsDirectory::setColorScheme(ColorScheme* colorScheme)
{
    m_colorScheme = colorScheme;

    for (auto tag : m_tagByID) {
        tag->setColorScheme(m_colorScheme);
    }
}

void TagsDirectory::tagNameChanged(QString text)
{
    auto tag = qobject_cast<Tag*>(sender());

    for (auto it = m_tagByName.begin(); it != m_tagByName.end(); ++it) {
        if (it.value().get() == tag) {
            m_tagByName.erase(it);
            break;
        }
    }

    if (!text.isEmpty()) {
        m_tagByName[text] = tag->shared_from_this();
    }
}

void TagsDirectory::tagFocusChanged(bool focused)
{
    auto tag = qobject_cast<Tag*>(sender());

    if (focused) {
        focusTag(tag->shared_from_this());
    } else {
        focusTag(nullptr);
    }
}

} // namespace tagberry::models
