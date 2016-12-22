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
#include "loader.h"

Loader::Loader(const std::string &path) : _dbf(path), _shp(path)
{
    if (this->_dbf.numEntities() != this->_shp.numEntities())
        throw std::runtime_error("dbf's entity and shp's entity number is different!");
    this->_n_entities = this->_dbf.numEntities();
    this->_current_index = 0;
}

bool Loader::consumeNext()
{
    try {
        Row row = this->_dbf.getRow(this->_current_index);
        switch (this->_shp.getShapeType()) {
        case ShapeType::Point:
        {
            Point point = this->_shp.getPoint(this->_current_index);
            this->_comsumer_set.point_consumer(this->_current_index, std::move(row), std::move(point));
            break;
        }
        case ShapeType::Polyline:
        {
            PolyLine polyline = this->_shp.getPolyline(this->_current_index);
            this->_comsumer_set.polyline_consumer(this->_current_index, std::move(row), std::move(polyline));
            break;
        }
        default:
            break;
        }
    } catch (const std::exception &e) {
        std::cerr << "exception(" << e.what() << ") in consuming index("
            << this->_current_index << "). SKip it." << std::endl;
    }

    return ++this->_current_index < this->_n_entities;
}

void Loader::designateValidFields(const std::vector<std::string> &field_names)
{
    this->_dbf.designateValidFields(field_names);
}

void Loader::registerConsumerSet(ConsumerSet &&consumer_set)
{
    this->_comsumer_set = std::move(consumer_set);
}
