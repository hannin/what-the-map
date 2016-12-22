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
#include "graph.h"
#include <iostream>

void Graph::print(const Graph::Way &way)
{
    auto it = way.cbegin();
    if (it != way.cend())
        std::cout << (*it)->id;

    while (++it != way.cend())
        std::cout << " -> " << (*it)->id;

    std::cout << std::endl;
}

// returns the way from 'from' to 'to' in shortest distance in sequence
Graph::Way Graph::get_shortest_path(AdjMap &adj_map, NodeId from, NodeId to)
{
    std::cout << "get shortest path from(" << from << ") to(" << to << ")" << std::endl;
    if (adj_map.link_map.find(from) == adj_map.link_map.end()) {
        throw std::invalid_argument("from node is not found in entries!");
    }

    // preprocessed vertices. pair<node id, distance>
    std::priority_queue<QueueNode, std::vector<QueueNode>, QueueNode> preps;

    // distances from the 'from' node
    std::map<NodeId, double> dists;

    std::map<NodeId, const Link *> parents;

    preps.emplace(from, 0);
    dists[from] = 0;

    while (!preps.empty()) {
        QueueNode u = preps.top();
        preps.pop();

        // node from the queue is not better than in dists map.
        // this queue node is already deprecated so just skip it
        auto dists_it = dists.find(u.id);
        if (dists_it != dists.end() && dists_it->second < u.dist)
            continue;

        // terminal node is found
        if (u.id == to) {
            std::cout << "terminal node(" << to << ") is found. let's build the way" << std::endl;
            // generate the way based on parents
            NodeId current = to;
            Graph::Way way;
            while (current != from) {
                const Link *entry = parents[current];
                way.push_front(entry);
                current = entry->from;
            }
            return way;
        }

        for (const Link *entry : adj_map.link_map[u.id]) {
            NodeId v = entry->to;
            double weight = entry->length();
            auto it = dists.find(v);
            if (it == dists.end() || it->second > u.dist + weight) {
                double v_dist = u.dist + weight;
                dists[v] = v_dist;
                preps.emplace(v, v_dist);
                parents[v] = entry;
            }
        }
    }

    throw std::invalid_argument("No way to go from.. to..");
}