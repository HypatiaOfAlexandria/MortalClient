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
#include "Geometry.h"

#include "GraphicsGL.h"

namespace jrc
{
void Geometry::draw(std::int16_t x,
                    std::int16_t y,
                    std::int16_t w,
                    std::int16_t h,
                    Geometry::Color cid,
                    float opacity) const
{
    if (w == 0 || h == 0 || opacity <= 0.0f)
        return;

    constexpr float colors[NUM_COLORS][3] = {
        {0.0f, 0.0f, 0.0f},  // Black
        {1.0f, 1.0f, 1.0f},  // White
        {0.0f, 1.0f, 0.0f},  // Mob-hp-bar lightgreen
        {0.0f, 0.75f, 0.0f}, // Mob-hp-bar green
        {0.0f, 0.5f, 0.0f}   // Mob-hp-bar darkgreen
    };
    const float* color = colors[cid];

    GraphicsGL::get().drawrectangle(
        x, y, w, h, color[0], color[1], color[2], opacity);
}

ColorBox::ColorBox(std::int16_t w, std::int16_t h, Geometry::Color c, float o)
    : width(w), height(h), color(c), opacity(o)
{
}

ColorBox::ColorBox() : ColorBox(0, 0, Geometry::BLACK, 0.0f)
{
}

void ColorBox::setwidth(std::int16_t w)
{
    width = w;
}

void ColorBox::setheight(std::int16_t h)
{
    height = h;
}

void ColorBox::set_color(Geometry::Color c)
{
    color = c;
}

void ColorBox::setopacity(float o)
{
    opacity = o;
}

void ColorBox::draw(const DrawArgument& args) const
{
    Point<std::int16_t> absp = args.getpos();
    std::int16_t absw = args.getstretch().x();
    if (absw == 0)
        absw = width;
    std::int16_t absh = args.getstretch().y();
    if (absh == 0)
        absh = height;
    absw = static_cast<std::int16_t>(absw * args.get_xscale());
    absh = static_cast<std::int16_t>(absh * args.get_yscale());
    float absopc = opacity * args.get_color().a();
    Geometry::draw(absp.x(), absp.y(), absw, absh, color, absopc);
}

ColorLine::ColorLine(std::int16_t w, Geometry::Color c, float o)
    : width(w), color(c), opacity(o)
{
}

ColorLine::ColorLine() : ColorLine(0, Geometry::BLACK, 0.0f)
{
}

void ColorLine::setwidth(std::int16_t w)
{
    width = w;
}

void ColorLine::set_color(Geometry::Color c)
{
    color = c;
}

void ColorLine::setopacity(float o)
{
    opacity = o;
}

void ColorLine::draw(const DrawArgument& args) const
{
    Point<std::int16_t> absp = args.getpos();
    std::int16_t absw = args.getstretch().x();
    if (absw == 0)
        absw = width;
    std::int16_t absh = args.getstretch().y();
    if (absh == 0)
        absh = 1;
    absw = static_cast<std::int16_t>(absw * args.get_xscale());
    absh = static_cast<std::int16_t>(absh * args.get_yscale());
    float absopc = opacity * args.get_color().a();
    Geometry::draw(absp.x(), absp.y(), absw, absh, color, absopc);
}

void MobHpBar::draw(Point<std::int16_t> position, std::int16_t hppercent) const
{
    std::int16_t fillw = static_cast<std::int16_t>((WIDTH - 6) *
                                         static_cast<float>(hppercent) / 100);
    std::int16_t x = position.x() - WIDTH / 2;
    std::int16_t y = position.y() - HEIGHT * 3;
    Geometry::draw(x, y, WIDTH, HEIGHT, BLACK, 1.0f);
    Geometry::draw(x + 1, y + 1, WIDTH - 2, 1, WHITE, 1.0f);
    Geometry::draw(x + 1, y + HEIGHT - 2, WIDTH - 2, 1, WHITE, 1.0f);
    Geometry::draw(x + 1, y + 2, 1, HEIGHT - 4, WHITE, 1.0f);
    Geometry::draw(x + WIDTH - 2, y + 2, 1, HEIGHT - 4, WHITE, 1.0f);
    Geometry::draw(x + 3, y + 3, fillw, 3, HPBAR_LIGHTGREEN, 1.0f);
    Geometry::draw(x + 3, y + 6, fillw, 1, HPBAR_DARKGREEN, 1.0f);
}
} // namespace jrc
