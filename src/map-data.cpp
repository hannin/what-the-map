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

#define GET_DIV_X(division_id) ((division_id) >> 3)
#define GET_DIV_Y(division_id) ((division_id) & MapData::_division_count)
#define GET_DIV(divx, divy) (((divx) << 3) | (divy))

const int MapData::_division_count = 0b111;

MapData::MapData()
    : link_loader(MOCT_LINK_PATH), node_loader(MOCT_NODE_PATH)
{
}

std::vector<int> MapData::getDivision(const Link &link)
{
    std::vector<int> division_ids;
    int xx = (link.polyline.box.x_max - box.x_min) / _division_x;
    int yy = (link.polyline.box.y_max - box.y_min) / _division_y;
    int division_id_upper_right = (xx << 3) | yy;

    division_ids.push_back(division_id_upper_right);

    xx = (link.polyline.box.x_min - box.x_min) / _division_x;
    yy = (link.polyline.box.y_min - box.y_min) / _division_y;
    int division_id_bottom_left = (xx << 3) | yy;

    if (division_id_upper_right == division_id_bottom_left)
        return division_ids;

    division_ids.push_back(division_id_bottom_left);
    if (GET_DIV_X(division_id_bottom_left) != GET_DIV_X(division_id_upper_right) &&
        GET_DIV_Y(division_id_bottom_left) != GET_DIV_Y(division_id_upper_right)) {
        division_ids.push_back(
            GET_DIV(
                GET_DIV_X(division_id_bottom_left) + 1,
                GET_DIV_Y(division_id_bottom_left)));
        division_ids.push_back(
            GET_DIV(
                GET_DIV_X(division_id_bottom_left),
                GET_DIV_Y(division_id_bottom_left) + 1));
    }

    return division_ids;
}

int MapData::getDivision(const Node &node)
{
    int xx = (node.point.x - box.x_min) / _division_x;
    int yy = (node.point.y - box.y_min) / _division_y;
    return (xx << 3) | yy;
}

Box MapData::getDivisionBox(int division)
{
    int xx = GET_DIV_X(division);
    int yy = GET_DIV_Y(division);

    Box division_box;
    division_box.x_max = box.x_min + _division_x * (xx + 1);
    division_box.x_min = box.x_min + _division_x * xx;
    division_box.y_max = box.y_min + _division_y * (yy + 1);
    division_box.y_min = box.y_min + _division_y * yy;
    return division_box;
}

void MapData::load()
{
    link_loader.load(adj_map);
    node_loader.load(adj_map);
    const Box &node_box = node_loader.getBox();
    const Box &link_box = link_loader.getBox();
    double minx = (node_box.x_min < link_box.x_min) ? node_box.x_min : link_box.x_min;
    double miny = (node_box.y_min < link_box.y_min) ? node_box.y_min : link_box.y_min;
    double maxx = (node_box.x_max > link_box.x_max) ? node_box.x_max : link_box.x_max;
    double maxy = (node_box.y_max > link_box.y_max) ? node_box.y_max : link_box.y_max;
    std::cout << "left bottom point: (" << minx << ", " << miny << ")" << std::endl;
    std::cout << "right top point: (" << maxx << ", " << maxy << ")" << std::endl;

    box.x_max = maxx;
    box.x_min = minx;
    box.y_max = maxy;
    box.y_min = miny;
}

void MapData::loadDivisionData()
{
    loadDivisionSide();
    adj_map.divided_map.clear();
    int division_id_max = ((_division_count << 3) | _division_count);
    for (auto &link : adj_map.raw_links) {
        auto divs = getDivision(*link);
        for (auto &division_id : divs) {
            if (division_id > division_id_max || division_id < 0) {
                throw std::runtime_error("something wrong on getting division id for link");
            } else {
                adj_map.divided_map[division_id].first.push_back(link.get());
            }
        }
    }

    for (auto &node : adj_map.node_map) {
        int division_id = getDivision(*node.second);
        if (division_id > division_id_max || division_id < 0) {
            std::cerr << "getting division id failed on node point: "
                << node.second->point.x << ", " << node.second->point.y << std::endl;
            //throw std::runtime_error("something wrong on getting division id for node");
        } else {
            adj_map.divided_map[division_id].second.push_back(node.second.get());
        }
    }
}

void MapData::loadDivisionSide()
{
    _division_x = (box.x_max - box.x_min) / _division_count;
    _division_y = (box.y_max - box.y_min) / _division_count;
}

void MapData::transformCoordinates(
    const CoordinateTransformer &x_transformer,
    const CoordinateTransformer &y_transformer,
    ReverseState reverse_state)
{
    auto box_transformer = [&](Box &box) {
        x_transformer(box.x_min);
        x_transformer(box.x_max);
        y_transformer(box.y_min);
        y_transformer(box.y_max);

        if (reverse_state & ReverseState::X) {
            std::swap(box.x_min, box.x_max);
            box.x_min = -box.x_min;
            box.x_max = -box.x_max;
        }
        if (reverse_state & ReverseState::Y) {
            std::swap(box.y_min, box.y_max);
            box.y_min = -box.y_min;
            box.y_max = -box.y_max;
        }
    };

    auto point_transformer = [&](Point &point) {
        x_transformer(point.x);
        y_transformer(point.y);

        if (reverse_state & ReverseState::X)
            point.x = -point.x;
        if (reverse_state & ReverseState::Y)
            point.y = -point.y;
    };

    // transform coordinates of global box
    box_transformer(this->box);

    // transform coordinates of links
    for (auto &link : this->adj_map.raw_links) {
        // to fix calculation of the length with untransformed coordinates
        link->length();

        box_transformer(link->polyline.box);
        for (auto &part : link->polyline.parts)
            for (auto &point : part)
                point_transformer(point);
    }

    // transform coordinates of nodes
    for (auto &node : this->adj_map.node_map)
        point_transformer(node.second->point);

    loadDivisionData();
}