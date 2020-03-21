/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <QPlainTextEdit>

class QMarkdownTextEdit;

namespace tagberry::widgets {

class MarkdownEdit : public QWidget {
    Q_OBJECT

public:
    explicit MarkdownEdit(QWidget* parent = nullptr);

private:
    QVBoxLayout m_layout;
    QPlainTextEdit* m_edit;
};

} // namespace tagberry::widgets
