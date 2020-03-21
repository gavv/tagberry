/*
 * Copyright (C) 2019 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "presenters/MainWindow.hpp"
#include "storage/LocalStorage.hpp"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QStandardPaths>
#include <QTime>

#include <iostream>

namespace {

void nullOutput(QtMsgType, const QMessageLogContext&, const QString&)
{
    return;
}

void stderrOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString time = QTime::currentTime().toString("HH:mm:ss.zzz");

    QString component;
    if (QString(context.category) != "default") {
        component = context.category;
    }
    if (context.file) {
        component = QFileInfo(context.file).baseName();
    }

    const char* level;
    switch (type) {
    case QtFatalMsg:
        level = "fatal";
        break;
    case QtCriticalMsg:
        level = "critical";
        break;
    case QtWarningMsg:
        level = "warning";
        break;
    case QtInfoMsg:
        level = "info";
        break;
    case QtDebugMsg:
    default:
        level = "debug";
        break;
    }

    QString line;

    line += time + " [";
    line += level;
    line += "] ";

    if (!component.isEmpty()) {
        line += "[" + component + "] ";
    }

    line += msg + "\n";

    std::cerr << line.toStdString();
}

QString defaultDBPath()
{
    auto config = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    if (!config.isEmpty()) {
        return QDir(config).filePath("tagberry-qt");
    }
    return QDir(QDir::homePath()).filePath(".tagberry-qt");
}

} // namespace

int main(int argc, char** argv)
{
    qInstallMessageHandler(nullOutput);

    QApplication app(argc, argv);

    app.setApplicationName("tagberry-qt");
    app.setWindowIcon(QIcon(":/icons/app.png"));

    QCommandLineParser parser;
    parser.setApplicationDescription("Tagberry Qt5 desktop app");

    QCommandLineOption helpOpt({ "h", "help" }, "Display help.");
    parser.addOption(helpOpt);

    QCommandLineOption dbOpt("db", "DB path.", "db", defaultDBPath());
    parser.addOption(dbOpt);

    if (!parser.parse(app.arguments())) {
        std::cerr << parser.errorText().toStdString();
        return 1;
    }

    if (parser.isSet(helpOpt)) {
        std::cerr << parser.helpText().toStdString();
        return 0;
    }

    qInstallMessageHandler(stderrOutput);

    tagberry::storage::LocalStorage storage;

    if (!storage.open(parser.value(dbOpt))) {
        return 1;
    }

    qDebug() << "initialization complete";

    tagberry::presenters::MainWindow window(storage);

    window.show();

    int code = app.exec();

    qDebug() << "exiting with code" << code;

    return code;
}
