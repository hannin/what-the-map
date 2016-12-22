// Copyright(C) 2016 Kyungwook Tak
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
#include <QApplication>
#include <Windows.h>
#include <stdexcept>
#include <cstdio>
#include <memory>
#include "main-window.h"

void alloc_console()
{
    if (AllocConsole() == 0)
        throw std::runtime_error("Failed to allocConsole...");

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

int main(int argc, char *argv[])
{
    alloc_console();

    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow window;
    window.show();

    return a.exec();
}