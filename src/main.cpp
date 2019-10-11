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

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    tagberry::storage::LocalStorage storage;

    if (!storage.open()) {
        return 1;
    }

    tagberry::presenters::MainWindow window(storage);

    window.show();

    return app.exec();
}
