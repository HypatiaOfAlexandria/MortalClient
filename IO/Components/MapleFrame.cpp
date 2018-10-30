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
#include "MapleFrame.h"

#include "nlnx/nx.hpp"

#include <algorithm>

namespace jrc
{
MapleFrame::MapleFrame(nl::node src)
{
    center = src["c"];
    east = src["e"];
    northeast = src["ne"];
    north = src["n"];
    northwest = src["nw"];
    west = src["w"];
    southwest = src["sw"];
    south = src["s"];
    southeast = src["se"];

    x_tile = std::max<std::int16_t>(north.width(), 1);
    y_tile = std::max<std::int16_t>(west.height(), 1);
}

MapleFrame::MapleFrame() = default;

void MapleFrame::draw(Point<std::int16_t> position,
                      std::int16_t rwidth,
                      std::int16_t rheight) const
{
    std::int16_t numhor = rwidth / x_tile + 2;
    std::int16_t numver = rheight / y_tile;
    std::int16_t width = numhor * x_tile;
    std::int16_t height = numver * y_tile;
    std::int16_t left = position.x() - width / 2;
    std::int16_t top = position.y() - height;
    std::int16_t right = left + width;
    std::int16_t bottom = top + height;

    northwest.draw({left, top});
    southwest.draw({left, bottom});
    for (std::int16_t y = top; y < bottom; y += y_tile) {
        west.draw({left, y});
        east.draw({right, y});
    }
    center.draw({{left, top}, {width, height}});
    for (std::int16_t x = left; x < right; x += x_tile) {
        north.draw({x, top});
        south.draw({x, bottom});
    }
    northeast.draw({right, top});
    southeast.draw({right, bottom});
}
} // namespace jrc
