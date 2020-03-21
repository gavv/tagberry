/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/RecordEdit.hpp"

namespace tagberry::widgets {

RecordEdit::RecordEdit(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout.addWidget(&m_cell);

    setLayout(&m_layout);

    m_completeCheckbox.setSize(24);

    m_titleEdit.setPlaceholderText("enter title");
    m_titleEdit.setFontWeight(QFont::Bold);

    m_titleRowLayout.setContentsMargins(QMargins(3, 3, 3, 3));
    m_titleRowLayout.setSpacing(4);
    m_titleRowLayout.addWidget(&m_completeCheckbox);
    m_titleRowLayout.addWidget(&m_titleEdit);
    m_titleRowLayout.setAlignment(&m_completeCheckbox, Qt::AlignTop);

    m_tagsRowLayout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_tagsRowLayout.addWidget(&m_tagListEdit);

    m_descRowLayout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_descRowLayout.addWidget(&m_descEdit);

    m_cell.setRowLayout(Row_Title, &m_titleRowLayout);
    m_cell.setRowLayout(Row_Tags, &m_tagsRowLayout);
    m_cell.setRowLayout(Row_Desc, &m_descRowLayout);

    connect(&m_cell, &MultirowCell::clicked, this, &RecordEdit::cellClicked);

    connect(&m_completeCheckbox, &CheckBox::clicked, this, [=] { clicked(this); });
    connect(
        &m_completeCheckbox, &CheckBox::stateChanged, this, &RecordEdit::completeChanged);

    connect(&m_titleEdit, &LineEdit::clicked, this, [=] { clicked(this); });
    connect(&m_titleEdit, &LineEdit::textChanged, this, &RecordEdit::titleChanged);
    connect(&m_titleEdit, &LineEdit::editingFinished, this,
        &RecordEdit::titleEditingFinished);

    connect(&m_tagListEdit, &TagListEdit::clicked, this, [=] { clicked(this); });
    connect(&m_tagListEdit, &TagListEdit::tagsChanged, this, &RecordEdit::tagsChanged);
    connect(&m_tagListEdit, &TagListEdit::tagAdded, this, &RecordEdit::tagAdded);
    connect(&m_tagListEdit, &TagListEdit::tagFocusChanged, this,
        &RecordEdit::tagFocusChanged);
    connect(&m_tagListEdit, &TagListEdit::tagFocusCleared, this,
        &RecordEdit::tagFocusCleared);

    connect(&m_descEdit, &MarkdownEdit::clicked, this, [=] { clicked(this); });
    connect(
        &m_descEdit, &MarkdownEdit::textChanged, this, &RecordEdit::descriptionChanged);
    connect(&m_descEdit, &MarkdownEdit::editingFinished, this,
        &RecordEdit::descriptionEditingFinished);
}

void RecordEdit::setTags(QList<TagLabel*> tags)
{
    m_tagListEdit.setTags(tags);
}

void RecordEdit::setComplete(bool complete)
{
    m_completeCheckbox.setChecked(complete);
}

void RecordEdit::setTitle(QString str)
{
    m_titleEdit.setText(str);
}

void RecordEdit::setDescription(QString str)
{
    m_descEdit.setText(str);
}

void RecordEdit::setFocused(bool focused)
{
    m_cell.setFocused(focused);
    m_titleEdit.setFocused(focused);
}

void RecordEdit::startEditing()
{
    m_titleEdit.startEditing();
}

bool RecordEdit::complete() const
{
    return m_completeCheckbox.isChecked();
}

QString RecordEdit::title() const
{
    return m_titleEdit.text();
}

QList<TagLabel*> RecordEdit::tags() const
{
    return m_tagListEdit.tags();
}

QString RecordEdit::description() const
{
    return m_descEdit.text();
}

void RecordEdit::notifyRemoving()
{
    removing();
}

void RecordEdit::cellClicked()
{
    tagFocusCleared();
    clicked(this);
}

void RecordEdit::setColors(QHash<QString, QColor> colors)
{
    m_cell.setBorderColor(colors["border"]);
    m_cell.setRowColor(Row_Title, colors["background"]);
    m_cell.setRowColor(Row_Tags, colors["background"]);
    m_cell.setRowColor(Row_Desc, colors["background"]);

    m_completeCheckbox.setColors(colors["background-dimmed"], colors["border"]);
    m_descEdit.setColors(colors);
}

} // namespace tagberry::widgets
