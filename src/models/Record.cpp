/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "models/Record.hpp"

namespace tagberry::models {

bool Record::isDirty() const
{
    if (m_isDirty) {
        return true;
    }
    for (auto tag : m_tags) {
        if (tag->isDirty()) {
            return true;
        }
    }
    return false;
}

void Record::unsetDirty()
{
    m_isDirty = false;
}

QString Record::id() const
{
    return m_id;
}

bool Record::hasID() const
{
    return !m_id.isEmpty();
}

void Record::setID(QString id)
{
    if (m_id == id) {
        return;
    }
    m_id = id;
    idChanged(id);
}

QDate Record::date() const
{
    return m_date;
}

void Record::setDate(QDate date)
{
    if (date == m_date) {
        return;
    }
    QDate oldDate = m_date;
    m_date = date;
    m_isDirty = true;
    dateChanged(oldDate, date);
}

QString Record::title() const
{
    return m_title;
}

void Record::setTitle(QString text)
{
    if (text == m_title) {
        return;
    }
    m_title = text;
    m_isDirty = true;
    textChanged(text);
}

QList<TagPtr> Record::tags() const
{
    return m_tags;
}

bool Record::hasTag(TagPtr tag) const
{
    return m_tags.indexOf(tag) != -1;
}

void Record::addTag(TagPtr tag)
{
    if (m_tags.indexOf(tag) != -1) {
        return;
    }
    m_isDirty = true;
    m_tags.append(tag);
    tagsChanged();
}

void Record::removeTag(TagPtr tag)
{
    if (m_tags.indexOf(tag) == -1) {
        return;
    }
    m_isDirty = true;
    m_tags.removeAll(tag);
    tagsChanged();
}

} // namespace tagberry::models
