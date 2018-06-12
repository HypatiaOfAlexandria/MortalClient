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
#include "Physics/PhysicsObject.h"

namespace jrc
{
struct Movement {
    enum Type { NONE, _ABSOLUTE, _RELATIVE, CHAIR, JUMPDOWN };

    Movement(Type t,
             std::uint8_t c,
             std::int16_t x,
             std::int16_t y,
             std::int16_t lx,
             std::int16_t ly,
             std::uint16_t f,
             std::uint8_t s,
             std::int16_t d)
        : type(t),
          command(c),
          xpos(x),
          ypos(y),
          lastx(lx),
          lasty(ly),
          fh(f),
          newstate(s),
          duration(d)
    {
    }

    Movement(std::int16_t x,
             std::int16_t y,
             std::int16_t lx,
             std::int16_t ly,
             std::uint8_t s,
             std::int16_t d)
        : Movement(_ABSOLUTE, 0, x, y, lx, ly, 0, s, d)
    {
    }

    Movement(const PhysicsObject& phobj, std::uint8_t s)
        : Movement(_ABSOLUTE,
                   0,
                   phobj.get_x(),
                   phobj.get_y(),
                   phobj.get_last_x(),
                   phobj.get_last_y(),
                   phobj.fh_id,
                   s,
                   1)
    {
    }

    Movement() : Movement(NONE, 0, 0, 0, 0, 0)
    {
    }

    bool hasmoved(const Movement& newmove) const
    {
        return newmove.newstate != newstate || newmove.xpos != xpos ||
               newmove.ypos != ypos || newmove.lastx != lastx ||
               newmove.lasty != lasty;
    }

    Type type;
    std::uint8_t command;
    std::int16_t xpos;
    std::int16_t ypos;
    std::int16_t lastx;
    std::int16_t lasty;
    std::uint16_t fh;
    std::uint8_t newstate;
    std::int16_t duration;
};
} // namespace jrc
