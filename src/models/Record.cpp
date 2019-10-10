/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "models/Record.hpp"

namespace tagberry::models {

Record::Record(QString id)
    : m_id(id)
{
}

QString Record::id() const
{
    return m_id;
}

QDate Record::date() const
{
    return m_date;
}

QString Record::text() const
{
    return m_text;
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
    m_tags.append(tag);
    tagsChanged();
}

void Record::removeTag(TagPtr tag)
{
    if (m_tags.indexOf(tag) == -1) {
        return;
    }
    m_tags.removeAll(tag);
    tagsChanged();
}

void Record::setDate(QDate date)
{
    if (date == m_date) {
        return;
    }
    QDate oldDate = m_date;
    m_date = date;
    dateChanged(oldDate, date);
}

void Record::setText(QString text)
{
    if (text == m_text) {
        return;
    }
    m_text = text;
    textChanged(text);
}

} // namespace tagberry::models
