/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "models/ColorScheme.hpp"

#include <algorithm>
#include <functional>
#include <string>

namespace tagberry::models {

namespace {

int tagIndex(const QString& name, int size)
{
    const size_t hash = std::hash<std::string>()(name.toStdString());
    const int index = int(hash % size_t(size));

    return index;
}

QColor shiftColor(const QColor c, int shift)
{
    const int r = std::min(255, shift + c.red());
    const int g = std::min(255, shift + c.green());
    const int b = std::min(255, shift + c.blue());

    return QColor(r, g, b);
}

} // namespace

ColorScheme::ColorScheme()
{
    setTagColors({
        "#ab6730",
        "#8A2F62",
        "#46573f",
        "#b02405",
        "#414f6b",
        "#974e45",
        "#69586e",
        "#573326",
        "#213438",
        "#4e6e14",
        "#3c75a3",
        "#479493",
        "#8f9140",
    });
}

QColor ColorScheme::tagRegularColor(const QString& name) const
{
    if (!m_tagColors.size()) {
        return {};
    }

    int index = tagIndex(name, m_tagColors.size());

    return m_tagColors[index];
}

QColor ColorScheme::tagFocusedColor(const QString& name) const
{
    if (!m_tagColors.size()) {
        return {};
    }

    int index = tagIndex(name, m_tagColors.size());

    return shiftColor(m_tagColors[index], 50);
}

void ColorScheme::setTagColors(QList<QColor> colors)
{
    m_tagColors = colors;
    colorsChanged();
}

} // namespace tagberry::models
