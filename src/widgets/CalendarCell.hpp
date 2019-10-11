/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "widgets/Cell.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

namespace tagberry::widgets {

class CalendarCell : public QWidget {
    Q_OBJECT

public:
    CalendarCell(QWidget* parent, int row, int col);

    int row() const;
    int column() const;

    QLayout* contentLayout();
    void setContentLayout(QLayout*);

    void setMonth(int);
    void setDay(int);

    void setFocused(bool);
    void setDimmed(bool);

    void setHeaderColor(QColor color);

signals:
    void clicked(CalendarCell*);

private:
    void updateTextColor();

    const int m_row;
    const int m_col;

    QHBoxLayout m_layout;
    Cell m_cell;

    QHBoxLayout m_headLayout;
    QLabel m_month;
    QLabel m_day;

    QColor m_normalTextColor;
    QColor m_dimmedTextColor;

    bool m_isDimmed;
};

} // namespace tagberry::widgets
