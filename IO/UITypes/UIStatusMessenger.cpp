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
#include "UIStatusMessenger.h"

#include "../../Constants.h"

namespace jrc
{
StatusInfo::StatusInfo(std::string&& str, Text::Color color)
{
    text = {Text::A12M, Text::RIGHT, color, std::string{str}};
    shadow = {Text::A12M, Text::RIGHT, Text::BLACK, std::move(str)};
    opacity.set(1.0f);
}

void StatusInfo::draw(Point<std::int16_t> position, float alpha) const
{
    float inter_opc = opacity.get(alpha);
    shadow.draw({position + 1, inter_opc});
    text.draw({position, inter_opc});
}

bool StatusInfo::update()
{
    static constexpr const float FADE_STEP
        = Constants::TIMESTEP * 1.0f / FADE_DURATION;

    opacity -= FADE_STEP;
    return opacity.last() < FADE_STEP;
}

UIStatusMessenger::UIStatusMessenger() noexcept
{
    position = {1020, 660};
}

void UIStatusMessenger::draw(float inter) const
{
    Point<std::int16_t> info_pos = {position.x(), position.y()};
    for (const StatusInfo& info : status_infos) {
        info.draw(info_pos, inter);
        info_pos.shift_y(-16);
    }
}

void UIStatusMessenger::update()
{
    for (StatusInfo& info : status_infos) {
        info.update();
    }
}

void UIStatusMessenger::show_status(Text::Color color, std::string&& message)
{
    status_infos.emplace_front(std::move(message), color);

    if (status_infos.size() > MAX_MESSAGES) {
        status_infos.pop_back();
    }
}
} // namespace jrc
