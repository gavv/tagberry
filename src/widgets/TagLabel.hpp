/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include <QColor>
#include <QFont>
#include <QString>
#include <QWidget>

namespace tagberry::widgets {

class TagLabel : public QWidget {
    Q_OBJECT

public:
    explicit TagLabel(QWidget* parent = nullptr);

    const QString& text() const;

    void setForegroundColor(const QColor& regular, const QColor& focused);
    void setBackgroundColor(const QColor&);

    void setFont(const QFont& font);
    void setPadding(int h, int v);
    void setRounding(int r);

public slots:
    void setText(QString text);
    void setCounter(int counter);

    void setFocused(bool);
    void setChecked(bool);

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void updateSize();

    QString m_text;
    QString m_counter;
    QColor m_fgRegular;
    QColor m_fgFocused;
    QColor m_bg;
    QFont m_font;

    int m_hPad;
    int m_vPad;
    int m_rounding;

    int m_cntSize;
    int m_w;
    int m_h;

    bool m_isFocused;
    bool m_isChecked;
};

} // tagberry::widgets
