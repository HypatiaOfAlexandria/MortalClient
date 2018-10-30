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
#include "ChatBalloon.h"

#include "../../Constants.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
ChatBalloon::ChatBalloon(std::int8_t type)
{
    std::string type_str = [type] {
        if (type < 0) {
            switch (type) {
            case -1:
                return std::string{"dead"};
                break;
            }
        } else {
            return std::to_string(type);
        }
    }();

    nl::node src = nl::nx::ui["ChatBalloon.img"][type_str];

    arrow = src["arrow"];
    frame = src;

    textlabel = {Text::A11M, Text::CENTER, Text::BLACK, u8"", 80};

    duration = 0;
}

ChatBalloon::ChatBalloon() : ChatBalloon{0}
{
}

void ChatBalloon::change_text(utf8_string&& text)
{
    textlabel.change_text(std::move(text));

    duration = DURATION;
}

void ChatBalloon::draw(Point<std::int16_t> position) const
{
    if (duration == 0) {
        return;
    }

    std::int16_t width = textlabel.width();
    std::int16_t height = textlabel.height();

    frame.draw(position, width, height);
    arrow.draw(position);
    textlabel.draw(position - Point<std::int16_t>(0, height + 4));
}

void ChatBalloon::update() noexcept
{
    duration -= Constants::TIMESTEP;
    if (duration < 0) {
        duration = 0;
    }
}

void ChatBalloon::expire() noexcept
{
    duration = 0;
}
} // namespace jrc
