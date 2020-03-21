/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/MarkdownEdit.hpp"

#include <QFontMetrics>

#include <qmarkdowntextedit.h>

namespace tagberry::widgets {

MarkdownEdit::MarkdownEdit(QWidget* parent)
    : QWidget(parent)
    , m_edit(new QPlainTextEdit)
{
    setLayout(&m_layout);

    m_layout.setContentsMargins(QMargins(6, 6, 6, 6));
    m_layout.addWidget(m_edit);

    m_edit->setFrameStyle(QFrame::NoFrame);

    auto font = m_edit->font();
    font.setPointSize(11);
    m_edit->setFont(font);
}

} // namespace tagberry::widgets
