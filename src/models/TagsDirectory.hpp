/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include "models/ColorScheme.hpp"
#include "models/Tag.hpp"

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>

namespace tagberry::models {

class TagsDirectory : public QObject {
    Q_OBJECT

public:
    QList<TagPtr> getTags() const;

    TagPtr getTag(const QString& name) const;

    TagPtr getOrCreateTag(const QString& name);

    void removeTag(TagPtr);

    void focusTag(TagPtr);

    void clearTags();

    void setColorScheme(ColorScheme*);

private slots:
    virtual void tagTextChanged(QString);
    virtual void tagFocusChanged(bool);

private:
    QHash<QString, TagPtr> m_tags;
    ColorScheme* m_colorScheme {};
};

} // namespace tagberry::models
