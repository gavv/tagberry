/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/RecordList.hpp"

#include <QMouseEvent>

namespace tagberry::widgets {

RecordList::RecordList(QWidget* parent)
    : QWidget(parent)
{
    m_scrollLayout.setContentsMargins(QMargins(0, 0, 2, 0));
    m_scrollLayout.setSpacing(10);
    m_scrollLayout.addStretch(1);

    m_scrollWidget.setLayout(&m_scrollLayout);

    m_scrollArea.setWidget(&m_scrollWidget);
    m_scrollArea.setWidgetResizable(true);
    m_scrollArea.setFrameShape(QFrame::NoFrame);

    m_buttonLayout.setContentsMargins(QMargins(0, 0, 2, 0));
    m_buttonLayout.addStretch(1);
    m_buttonLayout.addWidget(&m_removeRecordButton);
    m_buttonLayout.addWidget(&m_addRecordButton);

    m_layout.addLayout(&m_buttonLayout);
    m_layout.addSpacing(4);
    m_layout.addWidget(&m_scrollArea);

    setLayout(&m_layout);

    m_removeRecordButton.setFixedSize(34, 34);
    m_removeRecordButton.setIcon(QIcon(":/icons/minus.png"));

    m_addRecordButton.setFixedSize(34, 34);
    m_addRecordButton.setIcon(QIcon(":/icons/plus.png"));

    connect(
        &m_addRecordButton, &QPushButton::clicked, this, &RecordList::handleAddRecord);

    connect(&m_removeRecordButton, &QPushButton::clicked, this,
        &RecordList::handleRemoveRecord);

    m_scrollTimer.setSingleShot(true);
    m_scrollTimer.setInterval(5);

    alignHeader(0);
}

void RecordList::alignHeader(int hs)
{
    auto top = hs - m_addRecordButton.height() + 8;
    auto bottom = 7;

    m_layout.setContentsMargins(QMargins(0, top, 0, bottom));
}

void RecordList::clearCellFocus()
{
    cellChanged(nullptr);
}

void RecordList::cellChanged(RecordEdit* focusedCell)
{
    stopScrollTimer();

    m_focusedCell = focusedCell;

    for (auto cell : m_recordCells) {
        cell->setFocused(cell == focusedCell);
    }
}

void RecordList::addRecord(RecordEdit* record)
{
    connect(record, &RecordEdit::clicked, this, &RecordList::cellChanged);

    m_scrollLayout.insertWidget(m_scrollLayout.count() - 1, record);
    m_recordCells.append(record);
}

void RecordList::clearRecords()
{
    stopScrollTimer();

    for (auto record : m_recordCells) {
        m_scrollLayout.removeWidget(record);
        record->deleteLater();
    }

    m_recordCells.clear();
    m_focusedCell = nullptr;
}

void RecordList::handleAddRecord()
{
    auto record = new RecordEdit;

    addRecord(record);
    cellChanged(record);
    startScrollTimer(record);

    recordAdded(record);

    record->startEditing();
}

void RecordList::handleRemoveRecord()
{
    if (!m_focusedCell) {
        return;
    }

    int pos = m_recordCells.indexOf(m_focusedCell);

    m_scrollLayout.removeWidget(m_focusedCell);
    m_recordCells.removeAll(m_focusedCell);

    m_focusedCell->notifyRemoving();
    m_focusedCell->deleteLater();

    if (pos >= m_recordCells.count()) {
        pos = m_recordCells.count() - 1;
    }

    if (pos >= 0 && pos < m_recordCells.count()) {
        cellChanged(m_recordCells[pos]);
    } else {
        cellChanged(nullptr);
    }
}

void RecordList::mousePressEvent(QMouseEvent* event)
{
    if (clickedOutsideRecords(event)) {
        cellChanged(nullptr);
        tagFocusCleared();
    }
}

bool RecordList::clickedOutsideRecords(QMouseEvent* event)
{
    if (!m_scrollWidget.geometry().contains(event->pos())) {
        return true;
    }

    auto emptySpace = m_scrollLayout.itemAt(m_scrollLayout.count() - 1);

    if (!emptySpace) {
        return false;
    }

    return emptySpace->geometry().contains(event->pos());
}

void RecordList::startScrollTimer(RecordEdit* record)
{
    stopScrollTimer();

    connect(&m_scrollTimer, &QTimer::timeout, this, [=] {
        stopScrollTimer();
        m_scrollArea.ensureWidgetVisible(record);
    });

    m_scrollTimer.start();
}

void RecordList::stopScrollTimer()
{
    disconnect(&m_scrollTimer, nullptr, this, nullptr);
}

} // namespace tagberry::widgets
