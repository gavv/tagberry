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

signals:
    void pageChanged();

    void focusCleared();
    void focusChanged(TagLabel*);

private:
    void removeCellTags(CalendarCell*);

    QHBoxLayout* m_layout;
    Calendar* m_calendar;
};

} // namespace tagberry::widgets
