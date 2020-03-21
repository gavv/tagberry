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
#include <QFrame>
#include <QVector>
#include <QVBoxLayout>
#include <QWidget>

namespace tagberry::widgets {

class Cell : public QWidget {
    Q_OBJECT

public:
    explicit Cell(QWidget* parent = nullptr);

    QLayout* getRowLayout(int index);
    void setRowLayout(int index, QLayout* layout, int stretch = 0);

    void setFocused(bool);

    void setRowColor(int index, QColor color);
    void setBorderColor(QColor color);

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QVBoxLayout m_layout;

    QVector<QFrame*> m_rowFrames;
    QVector<QColor> m_rowColors;

    QColor m_borderColor { "#000000" };

    bool m_isFocused { false };
};

} // namespace tagberry::widgets
