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

namespace jrc
{
class Gauge
{
public:
    Gauge(Texture front,
          Texture mid,
          Texture end,
          std::int16_t maximum,
          float percentage) noexcept;
    Gauge();

    void draw(const DrawArgument& args) const;
    void update(float target);

private:
    Texture bar_front;
    Texture bar_mid;
    Texture bar_end;
    std::int16_t maximum;

    float percentage;
    float target;
    float step;
};
} // namespace jrc
