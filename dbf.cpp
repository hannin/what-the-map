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
#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
#include <shapefil.h>
#include "dbf.h"

Dbf::Dbf(const std::string &path) :
    _handle(static_cast<void *>(DBFOpen(path.c_str(), "rb")))
{
    if (this->_handle == nullptr)
        throw std::runtime_error("Failed to DBFOpen...");

    this->_n_fields = DBFGetFieldCount(static_cast<DBFHandle>(this->_handle));
    this->_n_entities = DBFGetRecordCount(static_cast<DBFHandle>(this->_handle));

    std::cout << "[" << path << "]" << std::endl;

    for (int i = 0; i < this->_n_fields; ++i) {
        std::vector<char> field_name_buf(12, '\0');
        DBFFieldType dbftype = DBFGetFieldInfo(
            static_cast<DBFHandle>(this->_handle),
            i,
            field_name_buf.data(), nullptr, nullptr);

        Dbf::Type type;
        switch (dbftype) {
        case FTString:
            type = Type::String;
            break;
        case FTInteger:
            type = Type::Integer;
            break;
        case FTDouble:
            type = Type::Double;
            break;
        case FTLogical:
            type = Type::Logical;
            break;
        case FTInvalid:
        default:
            type = Type::Invalid;
            break;
        }

        this->_fields.emplace_back(
            type, std::string(field_name_buf.data()));
    }
}

Dbf::~Dbf()
{
    DBFClose(static_cast<DBFHandle>(this->_handle));
}

void Dbf::designateValidFields(const std::vector<std::string> &field_names)
{
    for (size_t i = 0; i < this->_fields.size(); ++i) {
        for (const auto &field_name : field_names) {
            if (field_name == this->_fields[i].name) {
                std::cout << "field designated: " << field_name << std::endl;
                this->_designated_field_indices.push_back(i);
                break;
            }
        }
    }
}

std::string Dbf::getStringField(Index index, const std::string &field_name)
{
    int field_index = DBFGetFieldIndex(
        static_cast<DBFHandle>(this->_handle), field_name.c_str());
    if (field_index == -1)
        throw std::invalid_argument("invalid arg in Dbf::getStringField");

    const char *field_value = DBFReadStringAttribute(
        static_cast<DBFHandle>(this->_handle), index, field_index);
    if (field_value == nullptr || field_value[0] == '\0')
        return std::string();
    else
        return std::string(field_value);
}

Row Dbf::getRow(Index index)
{
    Row row;
    for (auto i : this->_designated_field_indices) {
        switch (this->_fields[i].type) {
        case Type::Double:
            row.doubles[this->_fields[i].name] =
                DBFReadDoubleAttribute(
                    static_cast<DBFHandle>(this->_handle),
                    static_cast<int>(index), static_cast<int>(i));
            break;

        case Type::Integer:
            row.integers[this->_fields[i].name] =
                DBFReadIntegerAttribute(
                    static_cast<DBFHandle>(this->_handle),
                    static_cast<int>(index), static_cast<int>(i));
            break;

        case Type::String:
        {
            const char *val = DBFReadStringAttribute(
                static_cast<DBFHandle>(this->_handle),
                static_cast<int>(index), static_cast<int>(i));
            row.strings[this->_fields[i].name] = (val == nullptr) ?
                std::string() : std::string(val);
            break;
        }

        default:
            throw std::invalid_argument("Unknown field type in dbf...");
        }
    }
    return row;
}

Dbf::Field::Field(Dbf::Type typeIn, std::string &&nameIn) :
    type(typeIn), name(std::move(nameIn))
{
}

Dbf::Field::Field(const Dbf::Field &other) : type(other.type), name(other.name) {}

Dbf::Field &Dbf::Field::operator=(const Dbf::Field &other)
{
    if (this == &other)
        return *this;

    this->type = other.type;
    this->name = other.name;

    return *this;
}

const char *Dbf::toString(const Type &type)
{
    switch (type) {
    case Type::String: return "String";
    case Type::Integer: return "Integer";
    case Type::Double: return "Double";
    case Type::Logical: return "Logical";
    case Type::Invalid:
    default:
        return "Invalid";
    }
}