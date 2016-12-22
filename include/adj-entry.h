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

#include <cstdint>
#include <vector>
#include <memory>
#include <map>
#include <cmath>
#include "base-objects.h"

struct Node {
    Node(Index i, Point &&p) : index(i), id(0), point(std::move(p)) {}

    Index index;
    NodeId id;
    Point point;
};

struct Link {
    Link(Index i, PolyLine &&p) :
        index(i), id(0), from(0), to(0), polyline(std::move(p)), _length(-1)
    {
    }

    ~Link() {}

    double length() const
    {
        if (_length != -1)
            return _length;

        // sum of all point to point in every parts
        double l = 0;
        for (const auto part : polyline.parts) {
            if (part.empty())
                continue;

            const Point *prev = &part[0];
            for (size_t i = 1; i < part.size(); ++i) {
                // calculated prev point and part[i]'s distance and add it to len
                l += std::sqrt(
                    std::pow((prev->x - part[i].x), 2) +
                    std::pow((prev->y - part[i].y), 2));
                prev = &part[i];
            }
        }

        return _length = l;
    }

    Index index;
    LinkId id;
    NodeId from;
    NodeId to;
    PolyLine polyline;


private:
    // to maintain const Link * in AdjMap, make it mutable
    mutable double _length; // length of the link
};

// MOCT_LINK makes directed graph(link) so data is not duplicated with one link.
// key: start node id(from)
// value: adjacent node list
struct AdjMap {
    // containing link resources and other maps refers here
    std::vector<std::unique_ptr<Link>> raw_links;

    // for finding adjacent list when find shortest path
    std::map<NodeId, std::vector<const Link *>> link_map;

    std::map<NodeId, std::unique_ptr<Node>> node_map;

    std::map<int, std::pair<std::vector<const Link *>, std::vector<const Node *>>> divided_map;

    AdjMap() {}
    AdjMap(const AdjMap &) = delete;
    AdjMap &operator=(const AdjMap &) = delete;
};