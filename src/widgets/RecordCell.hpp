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

class RecordCell : public QWidget {
    Q_OBJECT

public:
    explicit RecordCell(QWidget* parent = nullptr);

    QString title() const;
    QList<TagLabel*> tags() const;

    void setFocused(bool);

signals:
    void clicked(RecordCell*);

    void titleChanged(QString);
    void tagsChanged();
    void tagAdded(TagLabel*);

    void tagFocusChanged(TagLabel*);
    void tagFocusCleared();

private slots:
    void addTag();
    void removeTag();

    void cellClicked();
    void tagClicked();

    void updateTitle();
    void catchFocus(QWidget* old, QWidget* now);

protected:
    void showEvent(QShowEvent* event) override;

private:
    QHBoxLayout m_layout;
    Cell m_cell;

    QHBoxLayout m_headLayout;
    QTextEdit m_title;

    FlowLayout m_bodyLayout;
    QList<TagLabel*> m_tags;

    QPushButton m_addTag;

    bool m_firstShow { true };
};

} // namespace tagberry::widgets
