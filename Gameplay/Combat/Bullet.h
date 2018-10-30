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
#include "../../Graphics/Animation.h"
#include "../../Template/Point.h"
#include "../Physics/PhysicsObject.h"

namespace jrc
{
// Represents a projectile on a map.
class Bullet
{
public:
    Bullet(Animation animation, Point<std::int16_t> origin, bool toleft);

    void draw(double viewx, double viewy, float alpha) const;
    bool settarget(Point<std::int16_t> target);
    bool update(Point<std::int16_t> target);

private:
    Animation animation;
    MovingObject moveobj;
    bool flip;
};
} // namespace jrc
