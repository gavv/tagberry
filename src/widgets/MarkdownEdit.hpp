/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class QMarkdownTextEdit;

namespace tagberry::widgets {

class MarkdownEdit : public QWidget {
    Q_OBJECT

public:
    explicit MarkdownEdit(QWidget* parent = nullptr);

    QString text() const;
    void setText(const QString& str);

    void setPlaceholderText(const QString&);
    void setColors(const QHash<QString, QColor>& colors);

    void startEditing();

signals:
    void clicked();

    void textChanged(QString);
    void editingFinished(QString);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void updateText();
    void catchFocus(QWidget* old, QWidget* now);

private:
    QVBoxLayout m_layout;
    QMarkdownTextEdit* m_edit;
    QString m_lastText;

    int m_hMargin { 6 };
    int m_vMargin { 4 };
    int m_fontSize { 11 };
};

} // namespace tagberry::widgets
