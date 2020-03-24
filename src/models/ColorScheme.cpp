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

QColor fromHsl(int h, double s, double l)
{
    return QColor::fromHslF(h / 360.0, s, l);
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

int tagIndex(const QString& name, int size)
{
    auto str
        = QCryptographicHash::hash(name.toUtf8(), QCryptographicHash::Md5).toStdString();

    const size_t hash = std::hash<std::string>()(str);
    const int index = int(hash % size_t(size));

    return index;
}

} // namespace

ColorScheme::ColorScheme()
{
    m_widgetColors["background"] = "#ffffff";

    m_widgetColors["background-dimmed"] = "#f2f2f2";
    m_widgetColors["background-today"] = "#e4f5d7";

    m_widgetColors["text"] = "#000000";
    m_widgetColors["text-dimmed"] = "#999999";
    m_widgetColors["text-light"] = "#383838";
    m_widgetColors["text-extra-light"] = "#6e6e6e";
    m_widgetColors["text-url"] = "#3f7fa6";

    m_widgetColors["code-background"] = "#f5f5f5";
    m_widgetColors["code-keyword"] = fromHsl(301, 0.63, 0.40);
    m_widgetColors["code-type"] = fromHsl(41, 0.99, 0.38);
    m_widgetColors["code-builtin"] = fromHsl(221, 0.87, 0.60);
    m_widgetColors["code-string"] = fromHsl(119, 0.34, 0.47);
    m_widgetColors["code-number"] = fromHsl(41, 0.99, 0.30);
    m_widgetColors["code-comment"] = fromHsl(230, 0.99, 0.37);
    m_widgetColors["code-other"] = fromHsl(41, 0.99, 0.30);

    m_widgetColors["border"] = "#767C82";
    m_widgetColors["separator"] = "#99a0a3";

    m_builtinTagColors = {
        "#ab6730",
        "#8a2f62",
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
    };
}

QHash<QString, QColor> ColorScheme::widgetColors() const
{
    return m_widgetColors;
}

QHash<QString, QColor> ColorScheme::tagColors(const QString& name) const
{
    auto colors = m_tagsCache.object(name);
    if (colors) {
        return *colors;
    }

    colors = builtinTagColors(name);
    if (colors) {
        m_tagsCache.insert(name, colors);
        return *colors;
    }

    return {};
}

QHash<QString, QColor>* ColorScheme::builtinTagColors(const QString& name) const
{
    if (!m_builtinTagColors.size()) {
        return nullptr;
    }

    int index = tagIndex(name, m_builtinTagColors.size());

    auto color = m_builtinTagColors[index];

    return makeTagColors(color);
}

QHash<QString, QColor>* ColorScheme::makeTagColors(QColor baseColor) const
{
    auto colors = new QHash<QString, QColor>;

    (*colors)["background"] = m_widgetColors["background"];
    (*colors)["regular"] = baseColor;
    (*colors)["focused-complete"] = lightenColor(baseColor, 50);
    (*colors)["focused-incomplete"] = fadeColor(baseColor, 0.9f);

    return colors;
}

} // namespace tagberry::models
