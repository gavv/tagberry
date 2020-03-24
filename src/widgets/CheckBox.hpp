/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <QCheckBox>
#include <QVBoxLayout>
#include <QWidget>

namespace tagberry::widgets {

class CheckBox : public QWidget {
    Q_OBJECT

public:
    explicit CheckBox(QWidget* parent = nullptr);

    bool isChecked() const;
    void setChecked(bool);

    void setSize(int);
    void setColors(QColor background, QColor border);

signals:
    void stateChanged(bool);

private:
    QVBoxLayout m_layout;
    QCheckBox m_checkbox;
};

} // namespace tagberry::widgets
