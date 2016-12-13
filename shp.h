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

#include <string>
#include <functional>
#include <vector>

#include "base-objects.h"

class Shp {
public:
    Shp(const std::string &path);
    ~Shp();

    PolyLine getPolyline(Index index);
    Point getPoint(Index index);
    Index numEntities() const
    {
        return this->_n_entities;
    }
    ShapeType getShapeType() const
    {
        return this->_shape_type;
    }

    const Box &getBox() const
    {
        return this->_box;
    }

private:
    Index _n_entities;
    ShapeType _shape_type;
    void *_handle;
    Box _box;
};