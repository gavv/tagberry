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
        &m_root, &models::Root::currentDateChanged, this, &RecordsArea::rebuildRecords);

    connect(&m_recordList, &widgets::RecordList::recordAdded, this,
        &RecordsArea::recordAdded);

    connect(&m_recordList, &widgets::RecordList::focusCleared, this,
        [=] { m_root.tags().focusTag(nullptr); });

    setHeaderHeight(0);
    rebuildRecords();
}

void RecordsArea::setHeaderHeight(int h)
{
    m_recordList.alignHeader(h);
}

void RecordsArea::rebuildRecords()
{
    m_recordList.clearRecords();

    auto recordSet = m_root.currentPage().recordsByDate(m_root.currentDate());

    for (auto record : recordSet->getRecords()) {
        auto cell = new widgets::RecordCell;
        cell->setTitle(record->title());

        tagsFromModel(cell, record);
        bindRecord(cell, record);

        m_recordList.addRecord(cell);
    }

    resubscribeRecords();
}

void RecordsArea::resubscribeRecords()
{
    unsubscribeRecords();

    auto recSet = m_root.currentPage().recordsByDate(m_root.currentDate());

    m_subscribedRecordSet = recSet.get();

    connect(recSet.get(), &models::RecordSet::recordListChanged, this,
        &RecordsArea::rebuildRecords);
}

void RecordsArea::unsubscribeRecords()
{
    if (!m_subscribedRecordSet) {
        return;
    }

    disconnect(m_subscribedRecordSet, nullptr, this, nullptr);

    m_subscribedRecordSet.clear();
}

void RecordsArea::clearFocus()
{
    m_recordList.clearFocus();
}

void RecordsArea::recordAdded(widgets::RecordCell* cell)
{
    unsubscribeRecords();

    auto record = m_root.currentPage().createRecord();

    record->setDate(m_root.currentDate());

    bindRecord(cell, record);

    resubscribeRecords();
}

void RecordsArea::tagAdded(widgets::TagLabel* label)
{
    connect(label, &widgets::TagLabel::editingFinished, this, &RecordsArea::tagEdited);
}

void RecordsArea::tagEdited(QString oldText, QString newText)
{
    auto label = qobject_cast<widgets::TagLabel*>(sender());

    if (!oldText.isEmpty()) {
        auto oldTag = m_root.tags().getTagByName(oldText);

        if (oldTag) {
            disconnect(label, nullptr, oldTag.get(), nullptr);
            disconnect(oldTag.get(), nullptr, label, nullptr);
        }
    }

    auto newTag = m_root.tags().getTagByName(newText);
    if (!newTag) {
        newTag = m_root.tags().createTag();
        newTag->setName(label->text());
    }

    bindTag(label, newTag);
}

void RecordsArea::bindRecord(widgets::RecordCell* cell, models::RecordPtr record)
{
    connect(
        record.get(), &models::Record::textChanged, cell, &widgets::RecordCell::setTitle);

    connect(record.get(), &models::Record::tagsChanged, cell,
        [=] { tagsFromModel(cell, record); });

    connect(cell, &widgets::RecordCell::titleEditingFinished, record.get(),
        [=] { recordTitleToModel(cell, record); });

    connect(cell, &widgets::RecordCell::tagAdded, this, &RecordsArea::tagAdded);

    connect(cell, &widgets::RecordCell::tagFocusCleared,
        [=] { m_root.tags().focusTag(nullptr); });

    connect(cell, &widgets::RecordCell::tagsChanged, record.get(),
        [=] { tagsToModel(cell, record); });

    connect(cell, &widgets::RecordCell::removing, record.get(),
        [=] { removeRecord(record); });
}

void RecordsArea::bindTag(widgets::TagLabel* label, models::TagPtr tag)
{
    connect(tag.get(), &models::Tag::nameChanged, label, &widgets::TagLabel::setText);

    connect(tag.get(), &models::Tag::focusChanged, label,
        [=] { label->setFocused(tag->isFocused()); });

    connect(tag.get(), &models::Tag::colorsChanged, label, &widgets::TagLabel::setColors);

    auto colors = tag->getColors();

    label->setColors(std::get<0>(colors), std::get<1>(colors));
    label->setFocused(tag->isFocused());

    connect(label, &widgets::TagLabel::clicked, tag.get(),
        [=] { m_root.tags().focusTag(tag); });
}

void RecordsArea::tagsFromModel(widgets::RecordCell* cell, models::RecordPtr record)
{
    QList<widgets::TagLabel*> labelList;

    for (auto tag: record->tags()) {
        auto label = new widgets::TagLabel;
        label->setText(tag->name());

        tagAdded(label);
        bindTag(label, tag);

        labelList.append(label);
    }

    cell->setTags(labelList);
}

void RecordsArea::tagsToModel(widgets::RecordCell* cell, models::RecordPtr record)
{
    QList<models::TagPtr> tagList;

    for (auto label : cell->tags()) {
        auto tag = m_root.tags().getTagByName(label->text());
        if (!tag) {
            tag = m_root.tags().createTag();
            tag->setName(label->text());
        }

        tagList.append(tag);
    }

    record->setTags(tagList);

    m_storage.saveRecord(record);
}

void RecordsArea::recordTitleToModel(widgets::RecordCell* cell, models::RecordPtr record)
{
    record->setTitle(cell->title());

    m_storage.saveRecord(record);
}

void RecordsArea::removeRecord(models::RecordPtr record)
{
    unsubscribeRecords();

    m_storage.removeRecord(record);

    m_root.currentPage().removeRecord(record);

    resubscribeRecords();
}

} // namespace tagberry::presenters
