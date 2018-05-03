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
#include "../../Graphics/Geometry.h"
#include "../../Graphics/Text.h"
#include "../../Template/Interpolated.h"

namespace jrc
{
// The scrolling server notice at the top of the screen.
class ScrollingNotice
{
public:
    ScrollingNotice();

    void setnotice(std::string notice);
    void draw(float alpha) const;
    void update();

private:
    ColorBox background;
    Point<int16_t> backposition;
    Text notice;
    Linear<double> xpos;
    bool active;
};
} // namespace jrc
