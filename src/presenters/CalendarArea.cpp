/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "presenters/CalendarArea.hpp"

namespace tagberry::presenters {

CalendarArea::CalendarArea(storage::LocalStorage& storage, models::TagsDirectory& tagDir,
    models::RecordsDirectory& recDir)
    : m_layout(new QHBoxLayout)
    , m_calendar(new widgets::TagCalendar)
    , m_storage(storage)
    , m_tagDir(tagDir)
    , m_recDir(recDir)
{
    m_layout->setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout->addWidget(m_calendar);

    setLayout(m_layout);

    connect(
        m_calendar, &widgets::TagCalendar::pageChanged, this, &CalendarArea::refreshPage);

    connect(
        m_calendar, &widgets::TagCalendar::focusCleared, this, &CalendarArea::clearFocus);

    connect(m_calendar, &widgets::TagCalendar::focusChanged, this,
        &CalendarArea::changeFocus);

    refreshPage();
}

void CalendarArea::clearFocus()
{
    m_tagDir.focusTag(nullptr);
}

void CalendarArea::changeFocus(widgets::TagLabel* label)
{
    auto tag = m_tagDir.getTag(label->text());

    m_tagDir.focusTag(tag);
}

void CalendarArea::refreshPage()
{
    m_calendar->clearTags();

    m_recDir.clearRecords();
    m_tagDir.clearTags();

    auto range = m_calendar->getVisibleRange();

    for (auto date = range.first; date != range.second.addDays(1);
         date = date.addDays(1)) {
        auto recSet = m_recDir.recordsByDate(date);

        connect(recSet.get(), &models::RecordSet::recordTagsChanged,
            [=] { rebuildCell(date); });
    }

    m_storage.fillRecordsAndTags(range.first, range.second, m_recDir, m_tagDir);
}

void CalendarArea::rebuildCell(QDate date)
{
    m_calendar->clearTags(date);

    auto recSet = m_recDir.recordsByDate(date);

    for (auto tag : recSet->getAllTags()) {
        auto label = new widgets::TagLabel;

        label->setText(tag->name());
        label->setCounter(recSet->numRecordsWithTag(tag));

        connect(tag.get(), &models::Tag::nameChanged, label, &widgets::TagLabel::setText);

        connect(
            tag.get(), &models::Tag::focusChanged, label, &widgets::TagLabel::setFocused);

        m_calendar->addTag(date, label);
    }
}

} // namespace tagberry::presenters
