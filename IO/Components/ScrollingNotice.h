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
#include "../../Graphics/Geometry.h"
#include "../../Graphics/Text.h"
#include "../../Template/Interpolated.h"

namespace jrc
{
//! The scrolling server notice at the top of the screen.
class ScrollingNotice
{
public:
    ScrollingNotice();

    void set_notice(std::string&& notice) noexcept;
    void draw(float alpha) const;
    void update() noexcept;

private:
    ColorBox background;
    Point<std::int16_t> backposition;
    Text notice;
    Linear<double> xpos;
    bool active;
};
} // namespace jrc
