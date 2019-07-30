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
#pragma once

#include "../../Graphics/Text.h"
#include "../../Graphics/Texture.h"
#include "../../Template/BoolPair.h"
#include "nlnx/node.hpp"
#include "tinyutf8.hpp"

#include <vector>

namespace jrc
{
class Nametag
{
public:
    Nametag(nl::node src,
            Text::Font font,
            Text::Color color,
            utf8_string&& name);

    void draw(Point<std::int16_t> position) const;
    void set_selected(bool selected) noexcept;

private:
    Text name;
    BoolPair<std::vector<Texture>> textures;
    bool selected;
};
} // namespace jrc
