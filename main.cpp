/*
 * This file is part of LunaDash (https://github.com/LisaRowell/LunaDash)
 * Copyright (C) 2023 Lisa Rowell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Dashboard.h"

#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // For now we default to loading the file 'Dashboard.ldash' in the working
    // directory, but at some point this should change to a filename coming
    // from arguements and the user should be able to double click a .ldash
    // file to start running with that dashboard.
    const QString configFileName = "Dashboard.ldash";
    Dashboard dashboard(configFileName);
    dashboard.show();

    return application.exec();
}
