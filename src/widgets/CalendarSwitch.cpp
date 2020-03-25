/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "widgets/CalendarSwitch.hpp"

#include <QCompleter>
#include <QDate>
#include <QFontMetrics>
#include <QIcon>
#include <QLocale>
#include <QRegExpValidator>
#include <QStringList>

namespace tagberry::widgets {

namespace {

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
int horizontalAdvance(const QFontMetrics& m, const QString& s)
{
    return m.horizontalAdvance(s);
}
#else
int horizontalAdvance(const QFontMetrics& m, const QString& s)
{
    return m.width(s);
}
#endif

int editWidth(int numChars)
{
    QFontMetrics metrics(QFont {});
    QString s;
    for (int n = 0; n < numChars + 2; n++) {
        s += "0";
    }
    return horizontalAdvance(metrics, s);
}

QValidator* makeValidator(const QString& regexStr, QObject* parent)
{
    QRegExp regex { regexStr };
    regex.setCaseSensitivity(Qt::CaseInsensitive);
    return new QRegExpValidator(regex, parent);
}

QCompleter* makeCompleter(const QStringList& words, QObject* parent)
{
    auto completer = new QCompleter(words, parent);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    return completer;
}

QStringList monthNames()
{
    QLocale locale;
    QStringList ret;
    for (int month = 1; month <= 12; month++) {
        ret.append(locale.monthName(month, QLocale::LongFormat));
    }
    return ret;
}

} // namespace

CalendarSwitch::CalendarSwitch(QWidget* parent)
    : QWidget(parent)
    , m_prevMonth(new QPushButton)
    , m_nextMonth(new QPushButton)
    , m_editMonth(new QLineEdit)
    , m_prevYear(new QPushButton)
    , m_nextYear(new QPushButton)
    , m_editYear(new QLineEdit)
    , m_today(new QPushButton("TODAY"))
    , m_monthNames(monthNames())
    , m_year(-1)
    , m_month(-1)
{
    setLayout(&m_layout);

    m_layout.addWidget(m_prevMonth);
    m_layout.addWidget(m_editMonth);
    m_layout.addWidget(m_nextMonth);

    m_layout.addSpacing(50);

    m_layout.addWidget(m_prevYear);
    m_layout.addWidget(m_editYear);
    m_layout.addWidget(m_nextYear);

    m_layout.addSpacing(50);

    m_layout.addWidget(m_today);

    m_prevMonth->setIcon(QIcon(":/icons/arrow-left.png"));
    m_nextMonth->setIcon(QIcon(":/icons/arrow-right.png"));

    m_prevMonth->setIconSize(QSize(16, 16));
    m_nextMonth->setIconSize(QSize(16, 16));

    m_prevYear->setIcon(QIcon(":/icons/arrow-left.png"));
    m_nextYear->setIcon(QIcon(":/icons/arrow-right.png"));

    m_prevYear->setIconSize(QSize(16, 16));
    m_nextYear->setIconSize(QSize(16, 16));

    m_prevMonth->setFixedWidth(35);
    m_nextMonth->setFixedWidth(35);
    m_editMonth->setFixedWidth(editWidth(13));
    m_editMonth->setAlignment(Qt::AlignCenter);
    m_editMonth->setValidator(makeValidator("^(" + m_monthNames.join("|") + ")$", this));
    m_editMonth->setCompleter(makeCompleter(m_monthNames, this));

    m_prevYear->setFixedWidth(35);
    m_nextYear->setFixedWidth(35);
    m_editYear->setFixedWidth(editWidth(8));
    m_editYear->setAlignment(Qt::AlignCenter);
    m_editYear->setValidator(makeValidator("^[1-9][0-9]{3}$", this));

    QFont font;
    font.setPointSize(10);
    m_editMonth->setFont(font);
    m_editYear->setFont(font);

    font.setPointSize(8);
    font.setWeight(QFont::Bold);
    m_today->setFont(font);

    m_prevMonth->setFixedHeight(26);
    m_nextMonth->setFixedHeight(26);
    m_prevYear->setFixedHeight(26);
    m_nextYear->setFixedHeight(26);
    m_today->setFixedHeight(26);

    connect(m_today, &QPushButton::clicked, this, &CalendarSwitch::today);

    connect(m_prevMonth, &QPushButton::clicked, [=] {
        if (m_month > 1) {
            setYearMonth(m_year, m_month - 1);
        } else {
            setYearMonth(m_year - 1, 12);
        }
    });

    connect(m_nextMonth, &QPushButton::clicked, [=] {
        if (m_month < 12) {
            setYearMonth(m_year, m_month + 1);
        } else {
            setYearMonth(m_year + 1, 1);
        }
    });

    connect(
        m_prevYear, &QPushButton::clicked, [=] { setYearMonth(m_year - 1, m_month); });

    connect(
        m_nextYear, &QPushButton::clicked, [=] { setYearMonth(m_year + 1, m_month); });

    connect(m_editMonth, &QLineEdit::editingFinished, [=] {
        int month = 1;
        for (auto name : m_monthNames) {
            if (m_editMonth->text().compare(name, Qt::CaseInsensitive) == 0) {
                setYearMonth(m_year, month);
                return;
            }
            month++;
        }
    });

    connect(m_editYear, &QLineEdit::editingFinished,
        [=] { setYearMonth(m_editYear->text().toInt(), m_month); });
}

int CalendarSwitch::year() const
{
    return m_year;
}

int CalendarSwitch::month() const
{
    return m_month;
}

void CalendarSwitch::setYear(int year)
{
    setYearMonth(year, m_month);
}

void CalendarSwitch::setMonth(int month)
{
    setYearMonth(m_year, month);
}

void CalendarSwitch::setYearMonth(int year, int month)
{
    if (year == m_year && month == m_month) {
        return;
    }
    if (year < 1970 || year > 9999) {
        return;
    }
    if (month < 1 || month > 12) {
        return;
    }
    if (year != m_year) {
        m_editYear->setText(QString::number(year));
        m_year = year;
    }
    if (month != m_month) {
        m_editMonth->setText(m_monthNames[month - 1]);
        m_month = month;
    }
    switched(m_year, m_month);
}

} // namespace tagberry::widgets
