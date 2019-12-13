/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "widgets/RecordCell.hpp"

#include <QHBoxLayout>
#include <QList>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

namespace tagberry::widgets {

class RecordList : public QWidget {
    Q_OBJECT

public:
    explicit RecordList(QWidget* parent = nullptr);

    void alignHeader(int);

    void addRecord(RecordCell*);
    void clearRecords();

signals:
    void recordAdded(RecordCell*);

    void focusCleared();

public slots:
    void clearFocus();

private slots:
    void cellChanged(RecordCell*);

    void handleAddRecord();
    void handleRemoveRecord();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QVBoxLayout m_layout;

    QScrollArea m_scrollArea;
    QWidget m_scrollWidget;
    QVBoxLayout m_scrollLayout;

    QHBoxLayout m_buttonLayout;
    QPushButton m_addRecordButton;
    QPushButton m_removeRecordButton;

    QList<RecordCell*> m_recordCells;
    RecordCell* m_focusedCell {};
};

} // namespace tagberry::widgets
