//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "TwoSpriteButton.h"

namespace jrc
{
TwoSpriteButton::TwoSpriteButton(nl::node nsrc,
                                 nl::node ssrc,
                                 Point<int16_t> pos)
    : textures(ssrc, nsrc)
{
    position = pos;
    state = NORMAL;
    active = true;
}

TwoSpriteButton::TwoSpriteButton(nl::node nsrc, nl::node ssrc)
    : TwoSpriteButton(nsrc, ssrc, {})
{
}

TwoSpriteButton::TwoSpriteButton() : textures({}, {})
{
}

void TwoSpriteButton::draw(Point<int16_t> parentpos) const
{
    if (active) {
        bool selected = state == MOUSEOVER || state == PRESSED;
        textures[selected].draw(position + parentpos);
    }
}

Rectangle<int16_t> TwoSpriteButton::bounds(Point<int16_t> parentpos) const
{
    bool selected = state == MOUSEOVER || state == PRESSED;
    Point<int16_t> absp =
        parentpos + position - textures[selected].get_origin();
    Point<int16_t> dim = textures[selected].get_dimensions();
    return {absp, absp + dim};
}
} // namespace jrc
