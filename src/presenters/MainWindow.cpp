/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "presenters/MainWindow.hpp"

namespace tagberry::presenters {

MainWindow::MainWindow(storage::LocalStorage& storage)
    : m_storage(storage)
    , m_layout(new QHBoxLayout)
    , m_widget(new QWidget(this))
    , m_tagCalendarWidget(new widgets::TagCounterCalendar)
    , m_tagDir(new models::TagsDirectory(this))
    , m_recDir(new models::RecordsDirectory(this))
{
    m_layout->addWidget(m_tagCalendarWidget);
    m_layout->setContentsMargins(QMargins(0, 0, 0, 0));

    m_widget->setLayout(m_layout);
    setCentralWidget(m_widget);

    connect(m_tagCalendarWidget, &widgets::TagCounterCalendar::pageChanged, this,
        &MainWindow::refreshCalendar);

    connect(m_tagCalendarWidget, &widgets::TagCounterCalendar::focusCleared,
        [=] { setFocusedTag(nullptr); });

    connect(m_tagCalendarWidget, &widgets::TagCounterCalendar::focusChanged, this,
        &MainWindow::setFocusedTag);

    refreshCalendar();
}

void MainWindow::setFocusedTag(widgets::TagCounter* tagCounter)
{
    models::Tag* tag = nullptr;

    if (tagCounter) {
        tag = m_tagDir->getTag(tagCounter->text());
    }

    m_tagDir->focusTag(tag);
}

void MainWindow::refreshCalendar()
{
    m_tagCalendarWidget->clearTags();

    m_recDir->clearRecords();
    m_tagDir->clearTags();

    auto range = m_tagCalendarWidget->getVisibleRange();

    connectRecordSets(range.first, range.second);

    m_storage.fillRecordsAndTags(range.first, range.second, *m_recDir, *m_tagDir);
}

void MainWindow::refreshCalendarCell(QDate date)
{
    m_tagCalendarWidget->clearTags(date);

    auto recSet = m_recDir->recordsByDate(date);

    for (auto tag : recSet->getAllTags()) {
        auto tagCounter = new widgets::TagCounter;

        tagCounter->setText(tag->name());
        tagCounter->setCounter(recSet->numRecordsWithTag(tag));

        connect(
            tag, &models::Tag::nameChanged, tagCounter, &widgets::TagCounter::setText);

        connect(tag, &models::Tag::focusChanged, tagCounter,
            &widgets::TagCounter::setFocused);

        m_tagCalendarWidget->addTag(date, tagCounter);
    }
}

void MainWindow::connectRecordSets(QDate from, QDate to)
{
    for (auto date = from; date != to.addDays(1); date = date.addDays(1)) {
        auto recSet = m_recDir->recordsByDate(date);

        connect(recSet, &models::RecordSet::recordTagsChanged,
            [=] { refreshCalendarCell(date); });
    }
}

} // namespace tagberry::presenters
