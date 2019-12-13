/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "widgets/Cell.hpp"
#include "widgets/FlowLayout.hpp"
#include "widgets/TagLabel.hpp"

#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

namespace tagberry::widgets {

class TagSelector : public QWidget {
    Q_OBJECT

public:
    explicit TagSelector(QWidget* parent = nullptr);

    QList<TagLabel*> tags() const;

    void setTags(QList<TagLabel*>);

signals:
    void clicked();

    void tagsChanged();
    void tagAdded(TagLabel*);

    void tagFocusChanged(TagLabel*);
    void tagFocusCleared();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void handleTagAdd();
    void handleTagRemove();
    void handleTagUpdate(QString oldText, QString newText);
    void handleTagClick();
    void handleFocusChange(QWidget* old, QWidget* now);

private:
    void attachTag(TagLabel*);
    void detachTag(TagLabel*);

    void removeEmptyTags();

    FlowLayout m_layout;
    QList<TagLabel*> m_tags;

    QPushButton m_addTagButton;
};

} // namespace tagberry::widgets
