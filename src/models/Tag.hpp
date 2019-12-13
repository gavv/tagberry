/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "models/ColorScheme.hpp"

#include <QObject>
#include <QString>

#include <memory>
#include <tuple>

namespace tagberry::models {

class Tag : public QObject, public std::enable_shared_from_this<Tag> {
    Q_OBJECT

public:
    bool isDirty() const;
    void unsetDirty();

    QString id() const;
    bool hasID() const;
    void setID(QString);

    QString name() const;

    bool isFocused() const;

    std::tuple<QColor, QColor, QColor> getColors() const;
    void setColorScheme(ColorScheme*);

public slots:
    void setName(const QString&);
    void setFocused(bool);

signals:
    void idChanged(QString);
    void nameChanged(QString);
    void focusChanged(bool);
    void colorsChanged(std::tuple<QColor, QColor, QColor>);

private slots:
    void updateColors();

private:
    bool m_isDirty { true };

    QString m_id;
    QString m_name;
    bool m_focused { false };

    ColorScheme* m_colorScheme {};
};

using TagPtr = std::shared_ptr<Tag>;

} // namespace tagberry::models
