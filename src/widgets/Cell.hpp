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

    QLayout* headLayout();
    void setHeadLayout(QLayout*);

    QLayout* bodyLayout();
    void setBodyLayout(QLayout*);

    void setFocused(bool);

    void setHeaderColor(QColor header);

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QVBoxLayout m_layout;

    QFrame* m_headFrame;
    QFrame* m_bodyFrame;

    QColor m_bgColor;
    QColor m_borderColor;
    QColor m_headerColor;

    bool m_isFocused;
};

} // namespace tagberry::widgets
