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

#include <iostream>
#include <string>

#include "dbf.h"
#include "shp.h"
#include "adj-entry.h"
#include "base-objects.h"

class Loader {
public:
    Loader(const std::string &path);
    virtual void load(AdjMap &adj_map) = 0;
    const Box &getBox() const
    {
        return this->_shp.getBox();
    }

protected:
    using PolylineConsumer = std::function<void(Index index, Row &&row, PolyLine &&polyline)>;
    using PointConsumer = std::function<void(Index index, Row &&row, Point &&point)>;

    struct ConsumerSet {
        PolylineConsumer polyline_consumer;
        PointConsumer point_consumer;

        ConsumerSet() {}
        ConsumerSet(PolylineConsumer &&c1, PointConsumer &&c2) :
            polyline_consumer(std::move(c1)), point_consumer(std::move(c2))
        {
        }
    };

    void designateValidFields(const std::vector<std::string> &field_names);
    void registerConsumerSet(ConsumerSet &&consumer_set);
    bool consumeNext();
    Index numEntities() const
    {
        return this->_n_entities;
    }

    AdjMap _adj_map;
    Dbf _dbf;
    Shp _shp;

private:
    ConsumerSet _comsumer_set;
    Index _n_entities;
    Index _current_index;
};