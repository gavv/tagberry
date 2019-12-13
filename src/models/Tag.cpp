/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "models/Tag.hpp"

namespace tagberry::models {

bool Tag::isDirty() const
{
    return m_isDirty;
}

void Tag::unsetDirty()
{
    m_isDirty = false;
}

QString Tag::id() const
{
    return m_id;
}

bool Tag::hasID() const
{
    return !m_id.isEmpty();
}

void Tag::setID(QString id)
{
    if (id == m_id) {
        return;
    }
    m_id = id;
    idChanged(id);
}

QString Tag::name() const
{
    return m_name;
}

bool Tag::isFocused() const
{
    return m_focused;
}

void Tag::setName(const QString& text)
{
    if (text == m_name) {
        return;
    }
    m_isDirty = true;
    m_name = text;
    nameChanged(text);
    updateColors();
}

void Tag::setFocused(bool focused)
{
    if (focused == m_focused) {
        return;
    }
    m_focused = focused;
    focusChanged(focused);
}

std::tuple<QColor, QColor, QColor> Tag::getColors() const
{
    if (!m_colorScheme) {
        return {};
    };

    return m_colorScheme->tagColors(m_name);
}

void Tag::setColorScheme(ColorScheme* scheme)
{
    if (m_colorScheme) {
        disconnect(m_colorScheme, nullptr, this, nullptr);
    }

    m_colorScheme = scheme;

    if (m_colorScheme) {
        connect(m_colorScheme, &ColorScheme::colorsChanged, this, &Tag::updateColors);
    }

    updateColors();
}

void Tag::updateColors()
{
    if (!m_colorScheme) {
        return;
    }

    colorsChanged(getColors());
}

} // namespace tagberry::models
