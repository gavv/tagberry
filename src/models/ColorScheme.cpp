/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "models/ColorScheme.hpp"

#include <QCryptographicHash>

#include <algorithm>
#include <functional>
#include <string>

namespace tagberry::models {

namespace {

int tagIndex(const QString& name, int size)
{
    auto str
        = QCryptographicHash::hash(name.toUtf8(), QCryptographicHash::Md5).toStdString();

    const size_t hash = std::hash<std::string>()(str);
    const int index = int(hash % size_t(size));

    return index;
}

QColor lightenColor(const QColor c, int shift)
{
    const int r = std::min(255, shift + c.red());
    const int g = std::min(255, shift + c.green());
    const int b = std::min(255, shift + c.blue());

    return QColor(r, g, b);
}

QColor fadeColor(const QColor c, float fade)
{
    const int r = std::min(255, c.red() + int((255 - c.red()) * fade));
    const int g = std::min(255, c.green() + int((255 - c.green()) * fade));
    const int b = std::min(255, c.blue() + int((255 - c.blue()) * fade));

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

std::tuple<QColor, QColor, QColor> ColorScheme::tagColors(const QString& name) const
{
    if (!m_tagColors.size()) {
        return {};
    }

    int index = tagIndex(name, m_tagColors.size());

    auto color = m_tagColors[index];

    return std::make_tuple(color, lightenColor(color, 50), fadeColor(color, 0.9));
}

void ColorScheme::setTagColors(QList<QColor> colors)
{
    m_tagColors = colors;
    colorsChanged();
}

} // namespace tagberry::models
