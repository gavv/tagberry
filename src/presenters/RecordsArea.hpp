/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "models/Root.hpp"
#include "storage/LocalStorage.hpp"
#include "widgets/RecordList.hpp"

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

namespace tagberry::presenters {

class RecordsArea : public QWidget {
    Q_OBJECT

public:
    RecordsArea(storage::LocalStorage& storage, models::Root& root);

public slots:
    void setCurrentDate(QDate);

private slots:
    void recordAdded(widgets::RecordCell*);
    void tagAdded(widgets::TagLabel*);

private:
    QVBoxLayout m_layout;

    QLabel m_date;
    QScrollArea m_scroll;

    widgets::RecordList m_recordList;

    storage::LocalStorage& m_storage;
    models::Root& m_root;
};

} // namespace tagberry::presenters
