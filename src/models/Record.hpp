/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
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
    bool isDirty() const;
    void unsetDirty();

    QString id() const;
    bool hasID() const;
    void setID(QString);

    QDate date() const;
    bool complete() const;
    QString title() const;

    QList<TagPtr> tags() const;

    bool hasTag(TagPtr) const;
    void addTag(TagPtr);
    void removeTag(TagPtr);
    void setTags(QList<TagPtr>);

public slots:
    void setDate(QDate);
    void setComplete(bool);
    void setTitle(QString);

signals:
    void idChanged(QString);
    void dateChanged(QDate oldDate, QDate newDate);
    void completeChanged(bool);
    void textChanged(QString);
    void tagsChanged();

private:
    bool m_isDirty { true };
    QString m_id;
    QDate m_date;
    bool m_complete {};
    QString m_title;
    QList<TagPtr> m_tags;
};

using RecordPtr = std::shared_ptr<Record>;

} // namespace tagberry::models
