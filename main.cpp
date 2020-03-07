/* Copyright © 2013-2014 Paul DU
 * This file is part of Heavy Ball.

 * Heavy Ball is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Heavy Ball is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Heavy Ball.  If not, see <http://www.gnu.org/licenses/>. */

#include "mainwindow.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    MainWindow w;
    w.show();

    return a.exec();
}
