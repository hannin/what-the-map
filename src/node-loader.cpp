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
#include "node-loader.h"

void NodeLoader::load(AdjMap &adj_map)
{
    std::vector<std::string> valid_fields = { "NODE_ID", "NODE_NAME" };
    this->designateValidFields(valid_fields);

    this->registerConsumerSet(ConsumerSet(nullptr,
        [&adj_map, &valid_fields](Index index, Row &&row, Point &&point) {
        std::unique_ptr<Node> node(new Node(index, std::move(point)));
        try {
            node->id = static_cast<NodeId>(std::stoul(row.strings.at(valid_fields[0])));
        } catch (const std::invalid_argument &e) {
            std::cerr << "std::invalid_argument exception on stoul... "
                << "skip link. id:(" << row.strings.at(valid_fields[0]) << "). "
                << e.what() << std::endl;
            return;
        }

        adj_map.node_map[node->id] = std::move(node);
    }));

    std::cout << "reading node shp/dbf file..." << std::endl;
    while (this->consumeNext()) {}
}