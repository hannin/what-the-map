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

#include <vector>
#include <queue>
#include <list>
#include <utility>

#include "adj-entry.h"

class Graph {
public:
    using Way = std::list<const Link *>;
    static Way get_shortest_path(AdjMap &adj_map, NodeId from, NodeId to);
    static void print(const Way &);

private:
    // contains distance to the node(id) from node(get_shorted_path argument)
    struct QueueNode {
        NodeId id;
        double dist;
        QueueNode() : id(0), dist(0) {}
        QueueNode(NodeId nodeId, double distance) : id(nodeId), dist(distance) {}
        bool operator()(const QueueNode &lhs, const QueueNode &rhs) const
        {
            return lhs.dist > rhs.dist;
        }
    };
};