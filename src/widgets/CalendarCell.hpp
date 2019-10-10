/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include <QColor>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
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

    void setBorderColor(const QColor&);
    void setBackgroundColor(const QColor&);
    void setHeaderColor(const QColor&);
    void setTextColor(const QColor& normal, const QColor& dimmed);

signals:
    void clicked(CalendarCell*);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    void updateTextColor();

    const int m_row;
    const int m_col;

    QLabel* m_month;
    QLabel* m_day;

    QFrame* m_headFrame;
    QFrame* m_bodyFrame;

    QHBoxLayout m_headLayout;
    QVBoxLayout m_cellLayout;

    QColor m_bgColor;
    QColor m_borderColor;
    QColor m_headerColor;
    QColor m_normalTextColor;
    QColor m_dimmedTextColor;

    bool m_isFocused;
    bool m_isDimmed;
};

} // namespace tagberry::widgets
