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
#include "Text.h"

#include "GraphicsGL.h"

namespace jrc
{
Text::Text(Font f,
           Alignment a,
           Color c,
           Background b,
           const std::string& t,
           std::uint16_t mw,
           bool fm)
    : font(f),
      alignment(a),
      color(c),
      background(b),
      maxwidth(mw),
      formatted(fm)
{
    change_text(t);
}

Text::Text(Font f,
           Alignment a,
           Color c,
           const std::string& t,
           std::uint16_t mw,
           bool fm)
    : Text(f, a, c, NONE, t, mw, fm)
{
}

Text::Text() : Text(A11M, LEFT, BLACK)
{
}

void Text::reset_layout()
{
    if (text.empty()) {
        return;
    }

    layout = GraphicsGL::get().createlayout(
        text, font, alignment, maxwidth, formatted);
}

void Text::change_text(const std::string& t)
{
    if (text == t) {
        return;
    }

    text = t;

    reset_layout();
}

void Text::change_color(Color c)
{
    if (color == c) {
        return;
    }

    color = c;

    reset_layout();
}

void Text::set_background(Background b)
{
    background = b;
}

void Text::draw(const DrawArgument& args) const
{
    GraphicsGL::get().drawtext(args, text, layout, font, color, background);
}

std::uint16_t Text::advance(std::size_t pos) const
{
    return static_cast<std::uint16_t>(layout.advance(pos));
}

bool Text::empty() const
{
    return text.empty();
}

std::size_t Text::length() const
{
    return text.size();
}

std::int16_t Text::width() const
{
    return layout.width();
}

std::int16_t Text::height() const
{
    return layout.height();
}

Point<std::int16_t> Text::dimensions() const
{
    return layout.get_dimensions();
}

Point<std::int16_t> Text::endoffset() const
{
    return layout.get_endoffset();
}

const std::string& Text::get_text() const
{
    return text;
}

Text::Layout::Layout(const std::vector<Line>& l,
                     const std::vector<std::int16_t>& a,
                     std::int16_t w,
                     std::int16_t h,
                     std::int16_t ex,
                     std::int16_t ey)
    : lines(l), advances(a), dimensions(w, h), endoffset(ex, ey)
{
}

Text::Layout::Layout() : Layout({}, {}, 0, 0, 0, 0)
{
}

std::int16_t Text::Layout::width() const
{
    return dimensions.x();
}

std::int16_t Text::Layout::height() const
{
    return dimensions.y();
}

std::int16_t Text::Layout::advance(std::size_t index) const
{
    return static_cast<std::int16_t>(index < advances.size() ? advances[index]
                                                             : 0);
}

Point<std::int16_t> Text::Layout::get_dimensions() const
{
    return dimensions;
}

Point<std::int16_t> Text::Layout::get_endoffset() const
{
    return endoffset;
}

Text::Layout::iterator Text::Layout::begin() const
{
    return lines.begin();
}

Text::Layout::iterator Text::Layout::end() const
{
    return lines.end();
}
} // namespace jrc
