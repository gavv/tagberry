/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "widgets/MultirowCell.hpp"
#include "widgets/TagLabel.hpp"
#include "widgets/TagListEdit.hpp"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QTextEdit>
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

    void updateTitle();
    void catchFocus(QWidget* old, QWidget* now);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void updateStyleSheet(QColor bg, QColor border);

    enum { Row_Title = 0, Row_Tags = 1 };

    QHBoxLayout m_layout;
    MultirowCell m_cell;

    QHBoxLayout m_headerLayout;
    QCheckBox m_complete;
    QTextEdit m_title;

    QVBoxLayout m_bodyLayout;
    TagListEdit m_tagListEdit;

    bool m_firstPaint { true };

    QString m_lastTitle;
};

} // namespace tagberry::widgets
