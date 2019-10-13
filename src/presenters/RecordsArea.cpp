/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "presenters/RecordsArea.hpp"

namespace tagberry::presenters {

RecordsArea::RecordsArea(storage::LocalStorage& storage, models::Root& root)
    : m_storage(storage)
    , m_root(root)
{
    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout.addWidget(&m_recordList);

    setLayout(&m_layout);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    setMinimumWidth(200);

    connect(
        &m_root, &models::Root::currentDateChanged, this, &RecordsArea::setCurrentDate);

    connect(&m_recordList, &widgets::RecordList::recordAdded, this,
        &RecordsArea::recordAdded);

    setCurrentDate(m_root.currentDate());
    setHeaderHeight(0);
}

void RecordsArea::setHeaderHeight(int h)
{
    m_recordList.alignHeader(h);
}

void RecordsArea::setCurrentDate(QDate)
{
}

void RecordsArea::recordAdded(widgets::RecordCell* record)
{
    connect(record, &widgets::RecordCell::tagAdded, this, &RecordsArea::tagAdded);

    connect(record, &widgets::RecordCell::tagFocusCleared,
        [=] { m_root.tags().focusTag(nullptr); });
}

void RecordsArea::tagAdded(widgets::TagLabel* label)
{
    auto tag = m_root.tags().getOrCreateTag("tagID1");

    connect(tag.get(), &models::Tag::nameChanged, label, &widgets::TagLabel::setText);

    connect(tag.get(), &models::Tag::focusChanged, label,
        [=] { label->setFocused(tag->isFocused()); });

    connect(tag.get(), &models::Tag::colorsChanged, label, &widgets::TagLabel::setColors);

    auto colors = tag->getColors();

    label->setColors(std::get<0>(colors), std::get<1>(colors));
    label->setFocused(tag->isFocused());

    connect(label, &widgets::TagLabel::clicked, [=] { m_root.tags().focusTag(tag); });
}

} // namespace tagberry::presenters
