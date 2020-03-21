/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include "widgets/MultirowCell.hpp"

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
    void setToday(bool);

    void setColors(QHash<QString, QColor>);

signals:
    void clicked(CalendarCell*);

private:
    void updateCellColors();
    void updateTextColors();

    enum { Row_Header = 0, Row_Body = 1 };

    const int m_row;
    const int m_col;

    QHBoxLayout m_layout;
    MultirowCell m_cell;

    QHBoxLayout m_headLayout;
    QLabel m_month;
    QLabel m_day;

    QColor m_normalTextColor { "#000000" };
    QColor m_dimmedTextColor { "#000000" };

    QColor m_normalBackgroundColor { "#ffffff" };
    QColor m_todayBackgroundColor { "#00ff00" };

    QColor m_borderColor { "#000000" };

    bool m_isDimmed { false };
    bool m_isToday { false };
};

} // namespace tagberry::widgets
