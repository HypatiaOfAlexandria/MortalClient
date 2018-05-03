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
#include "../../Graphics/Animation.h"
#include "../../Template/Point.h"
#include "../Physics/PhysicsObject.h"

namespace jrc
{
// Represents a projectile on a map.
class Bullet
{
public:
    Bullet(Animation animation, Point<int16_t> origin, bool toleft);

    void draw(double viewx, double viewy, float alpha) const;
    bool settarget(Point<int16_t> target);
    bool update(Point<int16_t> target);

private:
    Animation animation;
    MovingObject moveobj;
    bool flip;
};
} // namespace jrc
