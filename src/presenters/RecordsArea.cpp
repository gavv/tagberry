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
    m_scroll.setWidget(&m_recordList);
    m_scroll.setWidgetResizable(true);
    m_scroll.setFrameShape(QFrame::NoFrame);

    m_layout.setContentsMargins(QMargins(0, 14, 0, 0));

    m_layout.addWidget(&m_date);
    m_layout.addWidget(&m_scroll);

    setLayout(&m_layout);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    setMinimumWidth(200);

    m_date.setAlignment(Qt::AlignCenter);
    m_date.setStyleSheet("font-weight: bold; border-style: solid; border-width: 1px; "
                         "border-color: #767C82; border-radius: 2px; padding: 3px; "
                         "margin-right: 5px; background-color: #ffffff;");

    connect(
        &m_root, &models::Root::currentDateChanged, this, &RecordsArea::setCurrentDate);

    connect(&m_recordList, &widgets::RecordList::recordAdded, this,
        &RecordsArea::recordAdded);

    setCurrentDate(m_root.currentDate());
}

void RecordsArea::setCurrentDate(QDate date)
{
    m_date.setText(date.toString(Qt::ISODate));
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

    connect(
        tag.get(), &models::Tag::focusChanged, label, &widgets::TagLabel::setFocused);

    connect(
        tag.get(), &models::Tag::colorsChanged, label, &widgets::TagLabel::setColors);

    auto colors = tag->getColors();

    label->setColors(std::get<0>(colors), std::get<1>(colors));
    label->setFocused(tag->isFocused());

    connect(label, &widgets::TagLabel::clicked, [=] { m_root.tags().focusTag(tag); });
}

} // namespace tagberry::presenters
