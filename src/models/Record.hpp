/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include "models/Tag.hpp"

#include <QDate>
#include <QList>
#include <QObject>
#include <QString>

namespace tagberry::models {

class Record : public QObject {
    Q_OBJECT

public:
    explicit Record(QString id, QObject* parent = nullptr);

    QString id() const;

    QDate date() const;

    QString text() const;

    QList<Tag*> tags() const;

    bool hasTag(Tag*) const;

    void addTag(Tag*);
    void removeTag(Tag*);

public slots:
    void setDate(QDate);
    void setText(QString);

signals:
    void dateChanged(QDate oldDate, QDate newDate);
    void textChanged(QString);
    void tagsChanged();

private:
    QString m_id;
    QDate m_date;
    QString m_text;
    QList<Tag*> m_tags;
};

} // tagberry::models
