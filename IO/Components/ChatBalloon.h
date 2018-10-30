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

#include "../../Graphics/Text.h"
#include "MapleFrame.h"
#include "tinyutf8.h"

namespace jrc
{
class ChatBalloon
{
public:
    ChatBalloon(std::int8_t type);
    ChatBalloon();

    void draw(Point<std::int16_t> position) const;
    void update() noexcept;

    void change_text(utf8_string&& text);
    void expire() noexcept;

private:
    //! How long a line stays on screen, in milliseconds.
    static constexpr std::int16_t DURATION = 4'000;

    MapleFrame frame;
    Text textlabel;
    Texture arrow;
    std::int16_t duration;
};
} // namespace jrc
