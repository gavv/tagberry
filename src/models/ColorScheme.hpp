/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <QCache>
#include <QColor>
#include <QHash>
#include <QList>
#include <QObject>
#include <QString>

namespace tagberry::models {

class ColorScheme : public QObject {
    Q_OBJECT

public:
    ColorScheme();

    QHash<QString, QColor> widgetColors() const;
    QHash<QString, QColor> tagColors(const QString& name) const;

signals:
    void widgetColorsChanged(QHash<QString, QColor>);
    void tagColorsChanged();

private:
    QHash<QString, QColor>* builtinTagColors(const QString& name) const;
    QHash<QString, QColor>* makeTagColors(QColor baseColor) const;

    QHash<QString, QColor> m_widgetColors;
    QList<QColor> m_builtinTagColors;
    mutable QCache<QString, QHash<QString, QColor>> m_tagsCache;
};

} // namespace tagberry::models
