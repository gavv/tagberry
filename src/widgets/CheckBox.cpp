/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/CheckBox.hpp"

#include <QApplication>

namespace tagberry::widgets {

CheckBox::CheckBox(QWidget* parent)
    : QWidget(parent)
{
    setLayout(&m_layout);

    m_layout.setContentsMargins(QMargins(0, 0, 0, 0));
    m_layout.addWidget(&m_checkbox);

    connect(
        &m_checkbox, &QCheckBox::stateChanged, this, [=] { stateChanged(isChecked()); });

    connect(qApp, &QApplication::focusChanged, this, &CheckBox::catchFocus);

    setColors("#ffffff", "#000000");
}

bool CheckBox::isChecked() const
{
    return m_checkbox.isChecked();
}

void CheckBox::setChecked(bool checked)
{
    m_checkbox.setChecked(checked);
}

void CheckBox::setSize(int size)
{
    m_checkbox.setFixedWidth(size);
}

void CheckBox::setColors(QColor background, QColor border)
{
    constexpr auto style = R"(
      QCheckBox::indicator {
        width: 17px;
        height: 17px;
        margin: 4px;
        border-style: solid;
        border-width: 1px;
        border-radius: 2px;
        border-color: %1;
      }
      QCheckBox::indicator:unchecked {
        image: none;
      }
      QCheckBox::indicator:checked {
        image: url(:/icons/mark.png);
        background-color: %2;
      }
)";
    m_checkbox.setStyleSheet(QString(style).arg(border.name(), background.name()));
}

void CheckBox::catchFocus(QWidget*, QWidget* now)
{
    if (now == &m_checkbox) {
        clicked();
    }
}

} // namespace tagberry::widgets
