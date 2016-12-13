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
#pragma once

#include <vector>
#include <map>
#include <string>
#include <list>

using Index = uint32_t;
using NodeId = uint32_t;
using LinkId = uint32_t;


// Base struct for defining boundary
struct Box {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
};

// Row for containing various dbf field variables
struct Row {
    std::map<std::string, std::string> strings;
    std::map<std::string, int> integers;
    std::map<std::string, double> doubles;
};

// SHP shape types
enum class ShapeType : int {
    Invalid = 0x00,
    Point = 0x01,
    Polyline = 0x03
};

// SHP type
struct Point {
    double x;
    double y;
};

// SHP type
struct PolyLine {
    Box box;
    std::vector<std::vector<Point>> parts;
};

