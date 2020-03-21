/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "widgets/CheckBox.hpp"
#include "widgets/LineEdit.hpp"
#include "widgets/MarkdownEdit.hpp"
#include "widgets/MultirowCell.hpp"
#include "widgets/TagLabel.hpp"
#include "widgets/TagListEdit.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

namespace tagberry::widgets {

class RecordEdit : public QWidget {
    Q_OBJECT

public:
    explicit RecordEdit(QWidget* parent = nullptr);

    bool complete() const;
    QString title() const;
    QList<TagLabel*> tags() const;

    void setTags(QList<TagLabel*>);

    void setFocused(bool);
    void startEditing();

    void notifyRemoving();

signals:
    void clicked(RecordEdit*);
    void removing();

    void completeChanged(bool);

    void titleChanged(QString);
    void titleEditingFinished(QString);

    void tagsChanged();
    void tagAdded(TagLabel*);

    void tagFocusChanged(TagLabel*);
    void tagFocusCleared();

public slots:
    void setComplete(bool);
    void setTitle(QString);
    void setColors(QHash<QString, QColor>);

private slots:
    void cellClicked();

private:
    enum { Row_Title = 0, Row_Tags = 1, Row_Text = 2 };

    QHBoxLayout m_layout;
    MultirowCell m_cell;

    QHBoxLayout m_titleRowLayout;
    CheckBox m_complete;
    LineEdit m_title;

    QVBoxLayout m_tagsRowLayout;
    TagListEdit m_tagListEdit;

    QVBoxLayout m_textRowLayout;
    MarkdownEdit m_textEdit;
};

} // namespace tagberry::widgets
