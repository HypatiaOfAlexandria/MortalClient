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
#include "MessagingHandlers.h"

#include "../../Character/Char.h"
#include "../../Data/ItemData.h"
#include "../../Gameplay/Stage.h"
#include "../../IO/Messages.h"
#include "../../IO/UI.h"
#include "../../IO/UITypes/UIStatusBar.h"
#include "../../IO/UITypes/UIStatusMessenger.h"

namespace jrc
{
// Modes:
// 0 - Item(0) or Meso(1)
// 3 - Exp gain
// 4 - Fame
// 5 - Mesos
// 6 - Guild points
void ShowStatusInfoHandler::handle(InPacket& recv) const
{
    int8_t mode = recv.read_byte();
    if (mode == 0) {
        int8_t mode2 = recv.read_byte();
        if (mode2 == 0) {
            int32_t itemid = recv.read_int();
            int32_t qty = recv.read_int();

            const ItemData& idata = ItemData::get(itemid);
            if (!idata.is_valid())
                return;

            std::string name = idata.get_name();
            std::string sign = (qty < 0) ? "-" : "+";

            show_status(Text::WHITE,
                        "Gained an item: " + name + " (" + sign +
                            std::to_string(qty) + ")");
        } else if (mode2 == 1) {
            recv.skip(1);

            int32_t gain = recv.read_int();
            std::string sign = (gain < 0) ? "-" : "+";

            show_status(Text::WHITE,
                        "Received mesos (" + sign + std::to_string(gain) +
                            ")");
        }
    } else if (mode == 3) {
        bool white = recv.read_bool();
        int32_t gain = recv.read_int();
        bool inchat = recv.read_bool();
        int32_t bonus1 = recv.read_int();

        recv.read_short();
        recv.read_int();  // bonus 2
        recv.read_bool(); // 'event or party'
        recv.read_int();  // bonus 3
        recv.read_int();  // bonus 4
        recv.read_int();  // bonus 5

        std::string message =
            "You have gained experience (+" + std::to_string(gain) + ")";
        if (inchat) {
        } else {
            show_status(white ? Text::WHITE : Text::YELLOW, message);
            if (bonus1 > 0)
                show_status(Text::YELLOW,
                            "+ Bonus EXP (+" + std::to_string(bonus1) + ")");
        }
    } else if (mode == 4) {
        int32_t gain = recv.read_int();
        std::string sign = (gain < 0) ? "-" : "+";

        show_status(Text::WHITE,
                    "Received fame (" + sign + std::to_string(gain) + ")");
    } else if (mode == 5) {
    }
}

void ShowStatusInfoHandler::show_status(Text::Color color,
                                        const std::string& message) const
{
    if (auto messenger = UI::get().get_element<UIStatusMessenger>())
        messenger->show_status(color, message);
}

void ServerMessageHandler::handle(InPacket& recv) const
{
    int8_t type = recv.read_byte();
    bool servermessage = recv.inspect_bool();
    if (servermessage)
        recv.skip(1);
    std::string message = recv.read_string();

    if (type == 3) {
        recv.read_byte(); // channel
        recv.read_bool(); // megaphone
    } else if (type == 4) {
        UI::get().set_scrollnotice(message);
    } else if (type == 7) {
        recv.read_int(); // npcid
    }
}

void WeekEventMessageHandler::handle(InPacket& recv) const
{
    recv.read_byte(); // always 0xFF in solaxia and moople
    std::string message = recv.read_string();

    static const std::string MAPLETIP = "[MapleTip]";
    if (message.substr(0, MAPLETIP.length()).compare("[MapleTip]"))
        message = "[Notice] " + message;

    UI::get().get_element<UIStatusbar>()->send_chatline(message,
                                                        UIChatbar::YELLOW);
}

void ChatReceivedHandler::handle(InPacket& recv) const
{
    int32_t charid = recv.read_int();
    recv.read_bool(); // 'gm'
    std::string message = recv.read_string();
    int8_t type = recv.read_byte();

    if (auto character = Stage::get().get_character(charid)) {
        message = character->get_name() + ": " + message;
        character->speak(message);
    }

    auto linetype = static_cast<UIChatbar::LineType>(type);
    if (auto statusbar = UI::get().get_element<UIStatusbar>())
        statusbar->send_chatline(message, linetype);
}

void ScrollResultHandler::handle(InPacket& recv) const
{
    int32_t cid = recv.read_int();
    bool success = recv.read_bool();
    bool destroyed = recv.read_bool();
    recv.read_short(); // legendary spirit if 1

    CharEffect::Id effect;
    Messages::Type message;
    if (success) {
        effect = CharEffect::SCROLL_SUCCESS;
        message = Messages::SCROLL_SUCCESS;
    } else {
        effect = CharEffect::SCROLL_FAILURE;
        if (destroyed) {
            message = Messages::SCROLL_DESTROYED;
        } else {
            message = Messages::SCROLL_FAILURE;
        }
    }

    Stage::get().show_character_effect(cid, effect);

    if (Stage::get().is_player(cid)) {
        if (auto statusbar = UI::get().get_element<UIStatusbar>())
            statusbar->display_message(message, UIChatbar::RED);

        UI::get().enable();
    }
}

void ShowItemGainInChatHandler::handle(InPacket& recv) const
{
    int8_t mode1 = recv.read_byte();
    if (mode1 == 3) {
        int8_t mode2 = recv.read_byte();
        if (mode2 == 1) // this actually is 'item gain in chat'
        {
            int32_t itemid = recv.read_int();
            int32_t qty = recv.read_int();

            const ItemData& idata = ItemData::get(itemid);
            if (!idata.is_valid())
                return;

            std::string name = idata.get_name();
            std::string sign = (qty < 0) ? "-" : "+";
            std::string message = "Gained an item: " + name + " (" + sign +
                                  std::to_string(qty) + ")";

            if (auto statusbar = UI::get().get_element<UIStatusbar>())
                statusbar->send_chatline(message, UIChatbar::BLUE);
        }
    } else if (mode1 == 13) // card effect
    {
        Stage::get().get_player().show_effect_id(CharEffect::MONSTER_CARD);
    } else if (mode1 == 18) // intro effect
    {
        recv.read_string(); // path
    } else if (mode1 == 23) // info
    {
        recv.read_string(); // path
        recv.read_int();    // some int
    } else                  // buff effect
    {
        int32_t skillid = recv.read_int();
        // more bytes, but we don't need them
        Stage::get().get_combat().show_player_buff(skillid);
    }
}
} // namespace jrc
