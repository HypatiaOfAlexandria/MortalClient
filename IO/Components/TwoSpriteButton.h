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
#include "../../Graphics/Texture.h"
#include "../../Template/BoolPair.h"
#include "Button.h"

#include <map>

namespace jrc
{
class TwoSpriteButton : public Button
{
public:
    TwoSpriteButton(nl::node nsrc,
                    nl::node ssrc,
                    Point<std::int16_t> position);
    TwoSpriteButton(nl::node nsrc, nl::node ssrc);
    TwoSpriteButton(Texture neg,
                    Texture selected,
                    Point<std::int16_t> position);
    TwoSpriteButton(Texture neg, Texture selected);
    TwoSpriteButton();

    void draw(Point<std::int16_t> position) const;
    Rectangle<std::int16_t> bounds(Point<std::int16_t> position) const;

private:
    BoolPair<Texture> textures;
};
} // namespace jrc
