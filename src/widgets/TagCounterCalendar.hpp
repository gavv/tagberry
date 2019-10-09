/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include "widgets/Calendar.hpp"
#include "widgets/TagCounter.hpp"

#include <QHBoxLayout>
#include <QWidget>

namespace tagberry::widgets {

class TagCounterCalendar : public QWidget {
    Q_OBJECT

public:
    explicit TagCounterCalendar(QWidget* parent = nullptr);

    QPair<QDate, QDate> getVisibleRange() const;

    QList<TagCounter*> getTags(const QDate& date);

    void addTag(const QDate& date, TagCounter* tag);
    void removeTag(const QDate& date, TagCounter* tag);

    void clearTags(const QDate& date);
    void clearTags();

signals:
    void pageChanged();

    void focusCleared();
    void focusChanged(TagCounter*);

private:
    void removeCellTags(CalendarCell*);

    QHBoxLayout* m_layout;
    Calendar* m_calendar;
};

} // tagberry::widgets
