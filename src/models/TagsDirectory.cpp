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

TagPtr TagsDirectory::createTag()
{
    auto tag = std::make_shared<Tag>();

    if (m_colorScheme) {
        tag->setColorScheme(m_colorScheme);
    }

    connect(tag.get(), &Tag::idChanged, this, &TagsDirectory::tagIdChanged);
    connect(tag.get(), &Tag::nameChanged, this, &TagsDirectory::tagNameChanged);
    connect(tag.get(), &Tag::focusChanged, this, &TagsDirectory::tagFocusChanged);

    m_tags.insert(tag);

    return tag;
}

TagPtr TagsDirectory::getOrCreateTag(const QString& id)
{
    if (auto tag = m_tagByID[id]) {
        return tag;
    }

    auto tag = createTag();
    tag->setID(id);

    return tag;
}

void TagsDirectory::removeTag(TagPtr tag)
{
    disconnect(tag.get(), nullptr, this, nullptr);

    m_tags.erase(tag);

    if (!tag->id().isEmpty()) {
        m_tagByID.remove(tag->id());
    }

    if (!tag->name().isEmpty()) {
        m_tagByName.remove(tag->name());
    }
}

void TagsDirectory::focusTag(TagPtr focusedTag)
{
    for (auto tag : m_tags) {
        tag->setFocused(tag == focusedTag);
    }
}

void TagsDirectory::clearTags()
{
    for (auto tag : m_tags) {
        disconnect(tag.get(), nullptr, this, nullptr);
    }

    m_tagByName.clear();
    m_tagByID.clear();

    m_tags.clear();
}

void TagsDirectory::setColorScheme(ColorScheme* colorScheme)
{
    m_colorScheme = colorScheme;

    for (auto tag : m_tags) {
        tag->setColorScheme(m_colorScheme);
    }
}

void TagsDirectory::tagIdChanged(QString id)
{
    auto tag = qobject_cast<Tag*>(sender());

    for (auto it = m_tagByID.begin(); it != m_tagByID.end(); ++it) {
        if (it.value().get() == tag) {
            m_tagByID.erase(it);
            break;
        }
    }

    if (!id.isEmpty()) {
        m_tagByID[id] = tag->shared_from_this();
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
    }
}

} // namespace tagberry::models
