/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>

namespace tagberry::widgets {

class CalendarSwitch : public QWidget {
    Q_OBJECT

public:
    explicit CalendarSwitch(QWidget* parent = nullptr);

    int year() const; // 1970..9999
    int month() const; // 1..12

    void setYear(int year);
    void setMonth(int month);
    void setYearMonth(int year, int month);

signals:
    void switched(int year, int month);
    void today();

private:
    QHBoxLayout m_layout;

    QPushButton* m_prevMonth;
    QPushButton* m_nextMonth;
    QLineEdit* m_editMonth;

    QPushButton* m_prevYear;
    QPushButton* m_nextYear;
    QLineEdit* m_editYear;

    QPushButton* m_today;

    QStringList m_monthNames;

    int m_year;
    int m_month;
};

} // namespace tagberry::widgets
