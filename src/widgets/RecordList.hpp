/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "widgets/RecordEdit.hpp"

#include <QHBoxLayout>
#include <QList>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

namespace tagberry::widgets {

class RecordList : public QWidget {
    Q_OBJECT

public:
    explicit RecordList(QWidget* parent = nullptr);

    void alignHeader(int);

    void addRecord(RecordEdit*);
    void clearRecords();

signals:
    void recordAdded(RecordEdit*);
    void tagFocusCleared();

public slots:
    void clearCellFocus();

private slots:
    void cellChanged(RecordEdit*);

    void handleAddRecord();
    void handleRemoveRecord();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    void startScrollTimer(RecordEdit* record, bool retry);
    void stopScrollTimer();

    bool clickedOutsideRecords(QMouseEvent* event);

    QVBoxLayout m_layout;

    QScrollArea m_scrollArea;
    QWidget m_scrollWidget;
    QVBoxLayout m_scrollLayout;

    QHBoxLayout m_buttonLayout;
    QPushButton m_addRecordButton;
    QPushButton m_removeRecordButton;

    QList<RecordEdit*> m_recordCells;
    RecordEdit* m_focusedCell {};

    QTimer m_scrollTimer;
};

} // namespace tagberry::widgets
