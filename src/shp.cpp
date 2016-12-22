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
#include <shapefil.h>
#include <string>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "shp.h"

namespace {

inline Point toPoint(SHPObject *object)
{
    Point point;
    point.x = object->padfX[0];
    point.y = object->padfY[0];
    return point;
}

inline PolyLine toPolyline(SHPObject *object)
{
    PolyLine polyline;

    polyline.box.x_max = object->dfXMax;
    polyline.box.x_min = object->dfXMin;
    polyline.box.y_max = object->dfYMax;
    polyline.box.y_min = object->dfYMin;

    if (object->nParts == 0 || object->nParts == 1) {
        std::vector<Point> part;
        for (int i = 0; i < object->nVertices; ++i) {
            Point point;
            point.x = object->padfX[i];
            point.y = object->padfY[i];
            part.emplace_back(std::move(point));
        }
        polyline.parts.emplace_back(std::move(part));
    } else {
        int part_index = 1;
        int next_part_start_index = object->panPartStart[part_index];
        std::vector<Point> part;
        for (int i = 0; i < object->nVertices; ++i) {
            Point point;
            point.x = object->padfX[i];
            point.y = object->padfY[i];
            part.emplace_back(std::move(point));

            if (i + 1 == next_part_start_index) {
                polyline.parts.emplace_back(std::move(part));
                part.clear();
                if (part_index + 1 == object->nParts)
                    next_part_start_index = object->nVertices;
                else
                    next_part_start_index = object->panPartStart[++part_index];
            }
        }
    }

    return polyline;
}

} // namespace anonymous

Shp::Shp(const std::string &path) :
    _n_entities(-1),
    _shape_type(ShapeType::Invalid),
    _handle(static_cast<void *>(SHPOpen(path.c_str(), "rb")))
{
    if (this->_handle == nullptr)
        throw std::runtime_error("Failed to SHPOpen...");

    int shape_type = static_cast<int>(ShapeType::Invalid);
    double min_bound[4] = { 0, };
    double max_bound[4] = { 0, };
    SHPGetInfo(
        static_cast<SHPHandle>(this->_handle),
        reinterpret_cast<int *>(&this->_n_entities),
        &shape_type, min_bound, max_bound);

    this->_shape_type = static_cast<ShapeType>(shape_type);

    if (this->_n_entities < 0) {
        std::cerr << "_n_entities is invalid: " << this->_n_entities << std::endl;
        throw std::runtime_error("SHPGetInfo returns invalid entity num...");
    }

    this->_box.x_min = min_bound[0];
    this->_box.x_max = max_bound[0];
    this->_box.y_min = min_bound[1];
    this->_box.y_max = max_bound[1];

    std::cout << "box xmin(" << this->_box.x_min << ") xmax(" << this->_box.x_max
        << ") ymin(" << this->_box.y_min << ") ymax(" << this->_box.y_max << ")" << std::endl;

    std::cout << "[" << path << "] shape type[" << static_cast<int>(this->_shape_type)
        << "] entity num[" << this->_n_entities << "]" << std::endl;
}

Shp::~Shp()
{
    SHPClose(static_cast<SHPHandle>(this->_handle));
}

PolyLine Shp::getPolyline(Index index)
{
    if (this->_shape_type != ShapeType::Polyline)
        throw std::runtime_error("this shape type isn't polyline!");

    std::unique_ptr<SHPObject, void(*)(SHPObject *)> object(
        SHPReadObject(
            static_cast<SHPHandle>(this->_handle),
            static_cast<int>(index)),
        SHPDestroyObject);

    if (object == nullptr)
        throw std::invalid_argument("index may be out of range");

    return toPolyline(object.get());
}

Point Shp::getPoint(Index index)
{
    if (this->_shape_type != ShapeType::Point)
        throw std::runtime_error("this shape type isn't point!");

    std::unique_ptr<SHPObject, void(*)(SHPObject *)> object(
        SHPReadObject(
            static_cast<SHPHandle>(this->_handle),
            static_cast<int>(index)),
        SHPDestroyObject);

    if (object == nullptr)
        throw std::invalid_argument("index may be out of range");

    return toPoint(object.get());
}