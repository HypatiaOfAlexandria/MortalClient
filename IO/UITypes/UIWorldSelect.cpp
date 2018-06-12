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
#include "UIWorldSelect.h"

#include "../../Configuration.h"
#include "../../Constants.h"
#include "../../Graphics/Sprite.h"
#include "../../IO/Components/MapleButton.h"
#include "../../IO/Components/TwoSpriteButton.h"
#include "../../IO/UI.h"
#include "../../Net/Packets/LoginPackets.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UIWorldSelect::UIWorldSelect(std::vector<World> worlds,
                             std::uint8_t worldcount)
    : UIElement({0, 0}, {Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT})
{
    worldid = Setting<DefaultWorld>::get().load();
    channelid = Setting<DefaultChannel>::get().load();

    nl::node back = nl::nx::map["Back"]["login.img"]["back"];
    nl::node worldsrc =
        nl::nx::ui["Login.img"]["WorldSelect"]["BtWorld"]["release"];
    nl::node channelsrc = nl::nx::ui["Login.img"]["WorldSelect"]["BtChannel"];
    nl::node frame = nl::nx::ui["Login.img"]["Common"]["frame"];

    sprites.emplace_back(back["11"], Point<std::int16_t>(370, 300));
    sprites.emplace_back(worldsrc["layer:bg"], Point<std::int16_t>(650, 45));
    sprites.emplace_back(frame, Point<std::int16_t>(400, 290));

    buttons[BT_ENTERWORLD] = std::make_unique<MapleButton>(
        channelsrc["button:GoWorld"], Point<std::int16_t>(200, 170));

    if (worldcount <= 0) {
        return;
    }

    const World& world = worlds.front();

    buttons[BT_WORLD0] = std::make_unique<MapleButton>(
        worldsrc["button:15"], Point<std::int16_t>(650, 20));
    buttons[BT_WORLD0]->set_state(Button::PRESSED);

    sprites.emplace_back(channelsrc["layer:bg"],
                         Point<std::int16_t>(200, 170));
    sprites.emplace_back(channelsrc["release"]["layer:15"],
                         Point<std::int16_t>(200, 170));

    if (channelid >= world.channel_count) {
        channelid = 0;
    }

    for (std::uint8_t i = 0; i < world.channel_count; ++i) {
        nl::node chnode = channelsrc["button:" + std::to_string(i)];
        buttons[BT_CHANNEL0 + i] =
            std::make_unique<TwoSpriteButton>(chnode["normal"]["0"],
                                              chnode["keyFocused"]["0"],
                                              Point<std::int16_t>{200, 170});
        if (i == channelid) {
            buttons[BT_CHANNEL0 + i]->set_state(Button::PRESSED);
        }
    }
}

void UIWorldSelect::draw(float alpha) const
{
    UIElement::draw(alpha);
}

std::uint8_t UIWorldSelect::get_world_id() const
{
    return worldid;
}

std::uint8_t UIWorldSelect::get_channel_id() const
{
    return channelid;
}

Button::State UIWorldSelect::button_pressed(std::uint16_t id)
{
    if (id == BT_ENTERWORLD) {
        UI::get().disable();

        CharlistRequestPacket(worldid, channelid).dispatch();

        return Button::PRESSED;
    } else if (id >= BT_WORLD0 && id < BT_CHANNEL0) {
        buttons[BT_WORLD0 + worldid]->set_state(Button::NORMAL);
        worldid = static_cast<std::uint8_t>(id - BT_WORLD0);
        return Button::PRESSED;
    } else {
        buttons[BT_CHANNEL0 + channelid]->set_state(Button::NORMAL);
        channelid = static_cast<std::uint8_t>(id - BT_CHANNEL0);
        return Button::PRESSED;
    }
}
} // namespace jrc
