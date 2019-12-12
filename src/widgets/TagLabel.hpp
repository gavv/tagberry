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
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

namespace tagberry::widgets {

class TagLabel : public QWidget {
    Q_OBJECT

public:
    explicit TagLabel(QWidget* parent = nullptr);

    const QString& text() const;

    bool isFocused() const;

    void setClosable(bool);
    void setEditable(bool);

    void setFont(const QFont& font);
    void setPadding(int h, int v);
    void setMargin(int h, int v);
    void setRounding(int r);

    void startEditing();

public slots:
    void setText(QString text);
    void setCustomIndicator(QString indicator);

    void setFocused(bool);
    void setComplete(bool);

    void setColors(QColor regular, QColor focused);

signals:
    void textChanged(QString);

    void clicked();
    void closeClicked();

    void editingStarted();
    void editingFinished(QString oldText, QString newText);

private slots:
    void finishEditing();

protected:
    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    void updateSizes();
    void updateColors();

    QVBoxLayout m_layout;
    QLineEdit* m_edit;

    QString m_oldText;
    QString m_text;
    QString m_customIndicator;
    bool m_closeButton;
    bool m_isEditable;

    QColor m_fgRegular;
    QColor m_fgFocused;
    QColor m_bg;
    QFont m_font;

    int m_hMargin;
    int m_vMargin;
    int m_hPad;
    int m_vPad;
    int m_textVertShift;
    int m_rounding;
    int m_cursorWidth;

    QRect m_innerRect;
    QRect m_closeRect;
    int m_indicatorWidth;

    bool m_isFocused;
    bool m_isComplete;
    bool m_isClosePressed;
    bool m_closePressStarted;
};

} // namespace tagberry::widgets
