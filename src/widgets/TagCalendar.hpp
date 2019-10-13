/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "widgets/Calendar.hpp"
#include "widgets/TagLabel.hpp"

#include <QHBoxLayout>
#include <QWidget>

namespace tagberry::widgets {

class TagCalendar : public QWidget {
    Q_OBJECT

public:
    explicit TagCalendar(QWidget* parent = nullptr);

    QPair<QDate, QDate> getVisibleRange() const;

    void addTag(const QDate& date, TagLabel* tag);

    void clearTags(const QDate& date);
    void clearTags();

    void setToday();

    int headerHeight();

signals:
    void pageChanged();

    void currentDateChanged(QDate);

    void tagFocusCleared();
    void tagFocusChanged(TagLabel*);

    void focusTaken();

private slots:
    void cellChanged(CalendarCell*);

private:
    void removeCellTags(CalendarCell*);

    QHBoxLayout* m_layout;
    Calendar* m_calendar;
};

} // namespace tagberry::widgets
