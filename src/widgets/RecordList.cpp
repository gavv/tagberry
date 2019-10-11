/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/RecordList.hpp"

namespace tagberry::widgets {

RecordList::RecordList(QWidget* parent)
    : QWidget(parent)
{
    m_buttonLayout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_buttonLayout.addStretch(1);
    m_buttonLayout.addWidget(&m_removeRecord);
    m_buttonLayout.addWidget(&m_addRecord);

    m_layout.setContentsMargins(QMargins(0, 10, 5, 10));
    m_layout.setSpacing(10);
    m_layout.addLayout(&m_buttonLayout);
    m_layout.addStretch(1);

    setLayout(&m_layout);

    m_removeRecord.setFixedSize(40, 40);
    m_removeRecord.setIcon(QIcon(":/icons/minus.png"));

    m_addRecord.setFixedSize(40, 40);
    m_addRecord.setIcon(QIcon(":/icons/plus.png"));

    connect(&m_addRecord, &QPushButton::clicked, this, &RecordList::addRecord);
    connect(&m_removeRecord, &QPushButton::clicked, this, &RecordList::removeRecord);
}

void RecordList::cellChanged(RecordCell* focusedCell)
{
    m_focusedCell = focusedCell;

    for (auto cell: m_records) {
        cell->setFocused(cell == focusedCell);
    }
}

void RecordList::addRecord()
{
    auto record = new RecordCell;

    connect(record, &RecordCell::clicked, this, &RecordList::cellChanged);

    m_layout.insertWidget(m_layout.count() - 2, record);
    m_records.append(record);

    cellChanged(record);
    recordAdded(record);
    recordListChanged();
}

void RecordList::removeRecord()
{
    if (!m_focusedCell) {
        return;
    }

    int pos = m_records.indexOf(m_focusedCell);

    m_layout.removeWidget(m_focusedCell);
    m_records.removeAll(m_focusedCell);

    delete m_focusedCell;

    if (pos >= m_records.count()) {
        pos = m_records.count() - 1;
    }

    if (pos >= 0 && pos < m_records.count()) {
        cellChanged(m_records[pos]);
    } else {
        cellChanged(nullptr);
    }

    recordListChanged();
}

} // namespace tagberry::widgets
