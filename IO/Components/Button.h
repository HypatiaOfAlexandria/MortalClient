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
#pragma once
#include "../../Template/Rectangle.h"
#include "../Cursor.h"

#include <cstdint>
#include <functional>

namespace jrc
{
// Base class for different button types.
class Button
{
public:
    enum State { NORMAL, DISABLED, MOUSEOVER, PRESSED, IDENTITY, NUM_STATES };

    virtual ~Button()
    {
    }

    virtual void draw(Point<std::int16_t> parentpos) const = 0;
    virtual Rectangle<std::int16_t> bounds(Point<std::int16_t> parentpos) const = 0;

    void set_position(Point<std::int16_t> position);
    void set_state(State state);
    void set_active(bool active);

    bool is_active() const;
    State get_state() const;

protected:
    State state;
    Point<std::int16_t> position;
    bool active;
};
} // namespace jrc
