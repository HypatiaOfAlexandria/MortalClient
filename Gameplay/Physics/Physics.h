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
#include "FootholdTree.h"

namespace jrc
{
// Class that uses physics engines and the collection of platforms to determine
// object movement.
class Physics
{
public:
    Physics(nl::node src);
    Physics();

    // Move the specified object over the specified game-time.
    void move_object(PhysicsObject& tomove) const;
    // Determine the point on the ground below the specified position.
    Point<std::int16_t> get_y_below(Point<std::int16_t> position) const;
    // Return a reference to the collection of platforms.
    const Footholdtree& get_fht() const;

private:
    void move_normal(PhysicsObject&) const;
    void move_flying(PhysicsObject&) const;
    void move_swimming(PhysicsObject&) const;

    Footholdtree fht;
};
} // namespace jrc
