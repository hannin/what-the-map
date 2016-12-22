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

#include <cmath>
#include <string>
#include <iostream>

#include "loader.h"

#define MOCT_LINK_PATH ".\\data\\MOCT_LINK"

class LinkLoader : public Loader {
public:
    LinkLoader(const std::string &path) : Loader(path) {}
    virtual void load(AdjMap &adj_map) override;

    PolyLine getPolyline(Index index)
    {
        return this->_shp.getPolyline(index);
    }

    Row getRow(Index index)
    {
        return this->_dbf.getRow(index);
    }
};