/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include <QObject>
#include <QString>

namespace tagberry::models {

class Tag : public QObject {
    Q_OBJECT

public:
    explicit Tag(QObject* parent = nullptr);

    QString name() const;
    bool focused() const;

public slots:
    void setName(const QString&);
    void setFocused(bool);

signals:
    void nameChanged(QString);
    void focusChanged(bool);

private:
    QString m_name;
    bool m_focused { false };
};

} // tagberry::models
