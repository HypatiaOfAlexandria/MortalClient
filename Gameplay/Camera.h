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
#include "../Template/Interpolated.h"
#include "../Template/Point.h"
#include "../Template/Range.h"

#include <cstdint>

namespace jrc
{
// View on stage which follows the player object.
class Camera
{
public:
    // Initialize everything to 0, we need the player's spawnpoint first to
    // properly set the position.
    Camera();

    // Update the view with the current player position. (Or any other target)
    void update(Point<std::int16_t> position);
    // Set the position, changing the view immediatly.
    void set_position(Point<std::int16_t> position);
    // Updates the view's boundaries. Determined by map_info or footholds.
    void set_view(Range<std::int16_t> hborders, Range<std::int16_t> vborders);
    // Return the current position.
    Point<std::int16_t> position() const;
    // Return the interpolated position.
    Point<std::int16_t> position(float alpha) const;
    // Return the interpolated position.
    Point<double> realposition(float alpha) const;

private:
    // Movement variables.
    Linear<double> x;
    Linear<double> y;

    // View limits.
    Range<std::int16_t> hbounds;
    Range<std::int16_t> vbounds;
};
} // namespace jrc
