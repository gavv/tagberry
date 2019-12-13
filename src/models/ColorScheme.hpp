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

#include <tuple>

namespace tagberry::models {

class ColorScheme : public QObject {
    Q_OBJECT

public:
    ColorScheme();

    std::tuple<QColor, QColor, QColor> tagColors(const QString& name) const;

    void setTagColors(QList<QColor>);

signals:
    void colorsChanged();

private:
    QList<QColor> m_tagColors;
};

} // namespace tagberry::models
