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
#include "Nametag.h"

namespace jrc
{
Nametag::Nametag(nl::node src, Text::Font f, Text::Color c, utf8_string&& n)
{
    name = {f, Text::CENTER, c};
    name.change_text(std::move(n));

    textures[false].push_back(src["0"]["0"]);
    textures[false].push_back(src["0"]["1"]);
    textures[false].push_back(src["0"]["2"]);
    textures[true].push_back(src["1"]["0"]);
    textures[true].push_back(src["1"]["1"]);
    textures[true].push_back(src["1"]["2"]);

    selected = false;
}

void Nametag::draw(Point<std::int16_t> position) const
{
    auto& tag = textures[selected];

    std::int16_t width = name.width();
    auto startpos = position - Point<std::int16_t>(8 + width / 2, -2);

    tag[0].draw(startpos);
    tag[1].draw(DrawArgument(startpos + Point<std::int16_t>(8, 0),
                             Point<std::int16_t>(width, 0)));
    tag[2].draw(DrawArgument(startpos + Point<std::int16_t>(width + 8, 0)));

    name.draw(position);
}

void Nametag::set_selected(bool s) noexcept
{
    selected = s;
}
} // namespace jrc
