//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <https://www.gnu.org/licenses/>.   //
//////////////////////////////////////////////////////////////////////////////
#include "Tile.h"

#include "nlnx/nx.hpp"

namespace jrc
{
Tile::Tile(nl::node src, std::string_view ts)
{
    nl::node dsrc = nl::nx::map["Tile"][ts][src["u"]][src["no"]];
    texture = Texture(nl::nx::map["Tile"][ts][src["u"]][src["no"]]);
    pos = Point<std::int16_t>(src["x"], src["y"]);
    z = dsrc["z"];
    if (z == 0) {
        z = dsrc["zM"];
    }
}

void Tile::draw(Point<std::int16_t> viewpos) const
{
    texture.draw(pos + viewpos);
}

std::uint8_t Tile::get_z() const
{
    return z;
}
} // namespace jrc
