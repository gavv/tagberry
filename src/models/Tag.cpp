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

    return std::make_tuple(m_colorScheme->backgroundColor(),
        m_colorScheme->tagRegularColor(m_name), m_colorScheme->tagFocusedColor(m_name));
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

    auto colors = getColors();

    colorsChanged(std::get<0>(colors), std::get<1>(colors), std::get<2>(colors));
}

} // namespace tagberry::models
