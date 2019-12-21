/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
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

class Cell : public QWidget {
    Q_OBJECT

public:
    explicit Cell(QWidget* parent = nullptr);

    QLayout* headerLayout();
    void setHeaderLayout(QLayout*);

    QLayout* bodyLayout();
    void setBodyLayout(QLayout*);

    QLayout* footerLayout();
    void setFooterLayout(QLayout*);

    void setFocused(bool);

    void setColors(QColor headerBackground, QColor bodyBackground, QColor border);

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QVBoxLayout m_layout;

    QFrame* m_headerFrame;
    QFrame* m_bodyFrame;
    QFrame* m_footerFrame;

    QColor m_headerColor;
    QColor m_bodyColor;
    QColor m_footerColor;
    QColor m_borderColor;

    bool m_isFocused;
};

} // namespace tagberry::widgets
