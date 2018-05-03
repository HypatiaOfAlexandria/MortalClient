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
#include "../../Graphics/Texture.h"
#include "../Cursor.h"
#include "TwoSpriteButton.h"

#include <functional>
#include <memory>

namespace jrc
{
class Slider
{
public:
    Slider(int32_t type,
           Range<int16_t> vertical,
           int16_t x,
           int16_t unitrows,
           int16_t rowmax,
           std::function<void(bool upwards)> onmoved);
    Slider();

    bool isenabled() const;
    void setenabled(bool enabled);
    void setrows(int16_t newrow, int16_t unitrows, int16_t rowmax);
    void setrows(int16_t unitrows, int16_t rowmax);
    void setvertical(Range<int16_t> vertical);

    void draw(Point<int16_t> position) const;
    bool remove_cursor(bool clicked);
    Cursor::State send_cursor(Point<int16_t> cursor, bool pressed);

private:
    Point<int16_t> getthumbpos() const;

    std::function<void(bool upwards)> onmoved;

    Range<int16_t> vertical;
    Point<int16_t> start;
    Point<int16_t> end;
    int16_t buttonheight;
    int16_t rowheight;
    int16_t x;
    int16_t row;
    int16_t rowmax;
    bool scrolling;
    bool enabled;

    Texture dbase;
    Texture dnext;
    Texture dprev;
    Texture base;
    TwoSpriteButton next;
    TwoSpriteButton prev;
    TwoSpriteButton thumb;
};
} // namespace jrc
