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
#include <map>

#include "base-objects.h"

class Dbf {
public:
    Dbf(const std::string &path);
    ~Dbf();

    std::string getStringField(Index index, const std::string &field_name);
    Row getRow(Index index);
    void designateValidFields(const std::vector<std::string> &field_names);

    Index numEntities() const
    {
        return this->_n_entities;
    }

private:
    enum class Type : int32_t {
        String,
        Integer,
        Double,
        Logical,
        Invalid
    };

    static const char *toString(const Type &type);

    struct Field {
        Type type;
        std::string name;

        Field() {}
        Field(Type type, std::string &&name);
        Field(const Field &);
        Field &operator=(const Field &);
    };

    std::vector<Field> _fields;
    std::vector<size_t> _designated_field_indices;

    Index _n_entities;
    int _n_fields;
    void *_handle;
};