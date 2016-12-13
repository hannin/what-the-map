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
#include "link-loader.h"

void LinkLoader::load(AdjMap &adj_map)
{
    std::vector<std::string> valid_fields = { "LINK_ID", "F_NODE", "T_NODE" };
    this->designateValidFields(valid_fields);

    this->registerConsumerSet(ConsumerSet(
        [&adj_map, &valid_fields](Index index, Row &&row, PolyLine &&polyline) {
        std::unique_ptr<Link> link(new Link(index, std::move(polyline)));
        try {
            link->id = static_cast<LinkId>(std::stoul(row.strings.at(valid_fields[0])));
            link->from = static_cast<NodeId>(std::stoul(row.strings.at(valid_fields[1])));
            link->to = static_cast<NodeId>(std::stoul(row.strings.at(valid_fields[2])));
        } catch (const std::invalid_argument &e) {
            std::cerr << "std::invalid_argument exception on stoul... "
                << "skip link. id:(" << row.strings.at(valid_fields[0]) << "). "
                << e.what() << std::endl;
            return;
        }

        adj_map.link_map[link->from].push_back(link.get());
        adj_map.raw_links.emplace_back(std::move(link));
    }, nullptr));

    std::cout << "reading link shp/dbf file..." << std::endl;
    while (this->consumeNext()) {}
}