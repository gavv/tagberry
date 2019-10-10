/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "models/Tag.hpp"

namespace tagberry::models {

QString Tag::name() const
{
    return m_name;
}

bool Tag::focused() const
{
    return m_focused;
}

void Tag::setName(const QString& text)
{
    if (text == m_name) {
        return;
    }
    m_name = text;
    nameChanged(text);
}

void Tag::setFocused(bool focused)
{
    if (focused == m_focused) {
        return;
    }
    m_focused = focused;
    focusChanged(focused);
}

} // tagberry::models
