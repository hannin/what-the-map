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

#include "link-loader.h"
#include "node-loader.h"
#include <vector>

struct MapData {
    LinkLoader link_loader;
    NodeLoader node_loader;
    AdjMap adj_map;
    Box box;

    MapData();

    std::vector<int> getDivision(const Link &link);
    int getDivision(const Node &node);
    Box getDivisionBox(int division);
    void load();

    // use x-ored this enum value to reverse coordinates
    enum ReverseState {
        NONE = 0,
        X = 0b01,
        Y = 0b10
    };
    using CoordinateTransformer = std::function<void(double &)>;
    void transformCoordinates(
        const CoordinateTransformer &x_transformer,
        const CoordinateTransformer &y_transformer,
        ReverseState reverse_state = ReverseState::NONE);

private:
    void loadDivisionData();
    void loadDivisionSide();

    static const int _division_count;
    double _division_x;
    double _division_y;
};