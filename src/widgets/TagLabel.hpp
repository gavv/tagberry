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
#include <QString>
#include <QWidget>

namespace tagberry::widgets {

class TagLabel : public QWidget {
    Q_OBJECT

public:
    explicit TagLabel(QWidget* parent = nullptr);

    const QString& text() const;

    void setFont(const QFont& font);
    void setPadding(int h, int v);
    void setRounding(int r);

public slots:
    void setText(QString text);
    void setCustomIndicator(QString indicator);

    void setFocused(bool);
    void setChecked(bool);

    void setColors(QColor regular, QColor focused);

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void updateSize();

    QString m_text;
    QString m_customIndicator;
    bool m_closeIndicator;

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

    QRect m_rect;
    int m_indicatorWidth;

    bool m_isFocused;
    bool m_isChecked;
};

} // namespace tagberry::widgets
