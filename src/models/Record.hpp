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

#include <memory>

namespace tagberry::models {

class Record : public QObject, public std::enable_shared_from_this<Record> {
    Q_OBJECT

public:
    explicit Record(QString id);

    QString id() const;

    QDate date() const;

    QString text() const;

    QList<TagPtr> tags() const;

    bool hasTag(TagPtr) const;

    void addTag(TagPtr);
    void removeTag(TagPtr);

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
    QList<TagPtr> m_tags;
};

using RecordPtr = std::shared_ptr<Record>;

} // tagberry::models
