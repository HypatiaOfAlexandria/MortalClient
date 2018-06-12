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
#include "DrawArgument.h"
#include "nlnx/bitmap.hpp"
#include "nlnx/node.hpp"

namespace jrc
{
//! Represents a single image loaded from a of game data.
class Texture
{
public:
    Texture(nl::node source);
    Texture();
    ~Texture();

    void draw(const DrawArgument& args) const;
    void shift(Point<std::int16_t> amount);

    bool is_valid() const;
    std::int16_t width() const;
    std::int16_t height() const;
    Point<std::int16_t> get_origin() const;
    Point<std::int16_t> get_dimensions() const;

private:
    nl::bitmap bitmap;
    Point<std::int16_t> origin;
    Point<std::int16_t> dimensions;
};
} // namespace jrc
