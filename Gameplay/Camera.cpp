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
#include "Camera.h"

#include "../Constants.h"

namespace jrc
{
Camera::Camera()
{
    x.set(0.0);
    y.set(0.0);
}

void Camera::update(Point<std::int16_t> position)
{
    double next_x = x.get();
    double hdelta = Constants::GAMEVIEWWIDTH / 2 - position.x() - next_x;
    if (std::abs(hdelta) >= 5.0) {
        next_x += hdelta * (12.0 / Constants::GAMEVIEWWIDTH);
    }

    double next_y = y.get();
    double vdelta = Constants::GAMEVIEWHEIGHT / 2 - position.y() - next_y;
    if (std::abs(vdelta) >= 5.0) {
        next_y += vdelta * (12.0 / Constants::GAMEVIEWHEIGHT);
    }

    if (next_x > hbounds.first() ||
        hbounds.length() < Constants::GAMEVIEWWIDTH) {
        next_x = hbounds.first();
    } else if (next_x < hbounds.second() + Constants::GAMEVIEWWIDTH) {
        next_x = hbounds.second() + Constants::GAMEVIEWWIDTH;
    }

    if (next_y > vbounds.first() ||
        vbounds.length() < Constants::GAMEVIEWHEIGHT) {
        next_y = vbounds.first();
    } else if (next_y < vbounds.second() + Constants::GAMEVIEWHEIGHT) {
        next_y = vbounds.second() + Constants::GAMEVIEWHEIGHT;
    }

    x = next_x;
    y = next_y;
}

void Camera::set_position(Point<std::int16_t> position)
{
    x.set(Constants::GAMEVIEWWIDTH / 2 - position.x());
    y.set(Constants::GAMEVIEWHEIGHT / 2 - position.y());
}

void Camera::set_view(Range<std::int16_t> mapwalls,
                      Range<std::int16_t> mapborders)
{
    hbounds = -mapwalls;
    vbounds = -mapborders;
}

Point<std::int16_t> Camera::position() const
{
    auto shortx = static_cast<std::int16_t>(std::round(x.get()));
    auto shorty = static_cast<std::int16_t>(std::round(y.get()));
    return {shortx, shorty};
}

Point<std::int16_t> Camera::position(float alpha) const
{
    auto interx = static_cast<std::int16_t>(std::round(x.get(alpha)));
    auto intery = static_cast<std::int16_t>(std::round(y.get(alpha)));
    return {interx, intery};
}

Point<double> Camera::realposition(float alpha) const
{
    return {x.get(alpha), y.get(alpha)};
}
} // namespace jrc
