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
#include <QPointer>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

namespace tagberry::presenters {

class RecordsArea : public QWidget {
    Q_OBJECT

public:
    RecordsArea(storage::LocalStorage& storage, models::Root& root);

    void setHeaderHeight(int);

public slots:
    void rebuildRecords();
    void clearFocus();

private slots:
    void recordAdded(widgets::RecordEdit*);
    void tagAdded(widgets::TagLabel*);
    void tagEdited(QString oldText, QString newText);

private:
    void resubscribeRecords();
    void unsubscribeRecords();

    void bindRecord(widgets::RecordEdit* cell, models::RecordPtr record);
    void bindTag(widgets::TagLabel* label, models::TagPtr tag);

    void tagsFromModel(widgets::RecordEdit* cell, models::RecordPtr record);
    void tagsToModel(widgets::RecordEdit* cell, models::RecordPtr record);

    void removeRecord(models::RecordPtr);

    QVBoxLayout m_layout;

    widgets::RecordList m_recordList;

    storage::LocalStorage& m_storage;
    models::Root& m_root;

    QPointer<models::RecordSet> m_subscribedRecordSet;
};

} // namespace tagberry::presenters
