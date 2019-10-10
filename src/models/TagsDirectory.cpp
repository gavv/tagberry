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

QList<TagPtr> TagsDirectory::getTags() const
{
    return m_tags.values();
}

TagPtr TagsDirectory::getTag(const QString& name) const
{
    return m_tags[name];
}

TagPtr TagsDirectory::getOrCreateTag(const QString& name)
{
    if (auto tag = m_tags[name]) {
        return tag;
    }

    auto tag = std::make_shared<Tag>();

    tag->setName(name);

    connect(tag.get(), &Tag::nameChanged, this, &TagsDirectory::tagTextChanged);
    connect(tag.get(), &Tag::focusChanged, this, &TagsDirectory::tagFocusChanged);

    m_tags[name] = tag;

    return tag;
}

void TagsDirectory::removeTag(TagPtr tag)
{
    disconnect(tag.get(), nullptr, this, nullptr);

    m_tags.remove(tag->name());
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

    m_tags.clear();
}

void TagsDirectory::tagTextChanged(QString text)
{
    auto tag = qobject_cast<Tag*>(sender());

    for (auto it = m_tags.begin(); it != m_tags.end(); ++it) {
        if (it.value().get() == tag) {
            m_tags.erase(it);
            break;
        }
    }

    m_tags[text] = tag->shared_from_this();
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

} // tagberry::models
