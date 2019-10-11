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
#include "models/Tag.hpp"

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>

#include <unordered_set>

namespace tagberry::models {

class TagsDirectory : public QObject {
    Q_OBJECT

public:
    QList<TagPtr> getTags() const;

    TagPtr getTagByName(const QString& name) const;

    TagPtr createTag();

    TagPtr getOrCreateTag(const QString& id);

    void removeTag(TagPtr);

    void focusTag(TagPtr);

    void clearTags();

    void setColorScheme(ColorScheme*);

private slots:
    virtual void tagIdChanged(QString);
    virtual void tagNameChanged(QString);
    virtual void tagFocusChanged(bool);

private:
    std::unordered_set<TagPtr> m_tags;

    QHash<QString, TagPtr> m_tagByName;
    QHash<QString, TagPtr> m_tagByID;

    ColorScheme* m_colorScheme {};
};

} // namespace tagberry::models
