/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/RecordList.hpp"

#include <QScrollBar>
#include <QTimer>

namespace tagberry::widgets {

RecordList::RecordList(QWidget* parent)
    : QWidget(parent)
{
    m_scrollWidget.setLayout(&m_scrollLayout);

    m_scroll.setWidget(&m_scrollWidget);
    m_scroll.setWidgetResizable(true);
    m_scroll.setFrameShape(QFrame::NoFrame);

    m_scrollLayout.setContentsMargins(QMargins(0, 0, 2, 0));
    m_scrollLayout.setSpacing(10);
    m_scrollLayout.addStretch(1);

    m_buttonLayout.setContentsMargins(QMargins(0, 0, 2, 0));
    m_buttonLayout.addStretch(1);
    m_buttonLayout.addWidget(&m_removeRecordButton);
    m_buttonLayout.addWidget(&m_addRecordButton);

    m_layout.addLayout(&m_buttonLayout);
    m_layout.addSpacing(4);
    m_layout.addWidget(&m_scroll);

    setLayout(&m_layout);

    m_removeRecordButton.setFixedSize(34, 34);
    m_removeRecordButton.setIcon(QIcon(":/icons/minus.png"));

    m_addRecordButton.setFixedSize(34, 34);
    m_addRecordButton.setIcon(QIcon(":/icons/plus.png"));

    connect(&m_addRecordButton, &QPushButton::clicked, this, &RecordList::addRecord);
    connect(&m_removeRecordButton, &QPushButton::clicked, this, &RecordList::removeRecord);

    alignHeader(0);
}

void RecordList::alignHeader(int hs)
{
    auto top = hs - m_addRecordButton.height() + 8;
    auto bottom = 7;

    m_layout.setContentsMargins(QMargins(0, top, 0, bottom));
}

void RecordList::cellChanged(RecordCell* focusedCell)
{
    m_focusedCell = focusedCell;

    for (auto cell : m_recordCells) {
        cell->setFocused(cell == focusedCell);
    }
}

void RecordList::addRecord()
{
    auto record = new RecordCell;

    connect(record, &RecordCell::clicked, this, &RecordList::cellChanged);

    m_scrollLayout.insertWidget(m_scrollLayout.count() - 1, record);
    m_recordCells.append(record);

    QTimer::singleShot(5, this, [=] { m_scroll.ensureWidgetVisible(record); });

    cellChanged(record);
    recordAdded(record);
    recordListChanged();
}

void RecordList::removeRecord()
{
    if (!m_focusedCell) {
        return;
    }

    int pos = m_recordCells.indexOf(m_focusedCell);

    m_scrollLayout.removeWidget(m_focusedCell);
    m_recordCells.removeAll(m_focusedCell);

    m_focusedCell->deleteLater();

    if (pos >= m_recordCells.count()) {
        pos = m_recordCells.count() - 1;
    }

    if (pos >= 0 && pos < m_recordCells.count()) {
        cellChanged(m_recordCells[pos]);
    } else {
        cellChanged(nullptr);
    }

    recordListChanged();
}

} // namespace tagberry::widgets
