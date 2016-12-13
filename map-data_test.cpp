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
#include "map-data.h"
#include <iostream>
#include <stdexcept>

void test_mapdata(void)
{
    MapData mapdata;
    mapdata.load();

    for (auto &link : mapdata.adj_map.raw_links) {
        // check parats non-emptiness
        if (link->polyline.parts.empty())
            throw std::invalid_argument("parts in polyline shouldn't be empty");

        // check box bound for polyline
        auto &box = link->polyline.box;
        for (auto &part : link->polyline.parts) {
            for (auto &point : part) {
                if (point.x > box.x_max || point.x < box.x_min ||
                    point.y > box.y_max || point.y < box.y_min)
                    throw std::invalid_argument("point in link isn't in polyline box");
            }
        }
    }
}