/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

namespace tagberry::widgets {

class LineEdit : public QWidget {
    Q_OBJECT

public:
    explicit LineEdit(QWidget* parent = nullptr);

    QString text() const;
    void setText(const QString& str);

    void setPlaceholderText(QString);
    void setFontWeight(QFont::Weight);
    void setFocused(bool);

    void startEditing();

signals:
    void clicked();

    void textChanged(QString);
    void editingFinished(QString);

private slots:
    void updateText();
    void catchFocus(QWidget* old, QWidget* now);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVBoxLayout m_layout;
    QTextEdit m_edit;
    bool m_firstPaint { true };
    QString m_lastText;
};

} // namespace tagberry::widgets
