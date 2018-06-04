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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../../Graphics/Animation.h"
#include "../../Template/Point.h"

namespace jrc
{
// Represents an obj (map decoration) on a map.
class Obj
{
public:
    Obj(nl::node source);

    // Update animation.
    void update();
    // Draw the obj at the specified position.
    void draw(Point<std::int16_t> viewpos, float inter) const;
    // Return depth of the obj.
    std::uint8_t getz() const;

private:
    Animation animation;
    Point<std::int16_t> pos;
    std::uint8_t z;
    bool flip;
};
} // namespace jrc
