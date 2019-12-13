/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <QColor>
#include <QList>
#include <QObject>
#include <QString>
#include <QHash>

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
    QHash<QString, QColor> m_widgetColors;
    QList<QColor> m_tagColors;
};

} // namespace tagberry::models
