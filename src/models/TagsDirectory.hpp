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

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>

namespace tagberry::models {

class TagsDirectory : public QObject {
    Q_OBJECT

public:
    explicit TagsDirectory(QObject* parent = nullptr);

    QList<Tag*> getTags() const;

    Tag* getTag(const QString& name) const;

    Tag* getOrCreateTag(const QString& name);

    void removeTag(Tag*);

    void focusTag(Tag*);

    void clearTags();

private slots:
    virtual void tagTextChanged(QString);
    virtual void tagFocusChanged(bool);

private:
    QHash<QString, Tag*> m_tags;
};

} // tagberry::models
