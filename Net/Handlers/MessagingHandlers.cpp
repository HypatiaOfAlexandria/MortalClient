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
//! ## Modes:
//!
//! - 0 : Item(0) or Meso(1)
//! - 3 : EXP gain
//! - 4 : Fame
//! - 5 : Mesos
//! - 6 : Guild points
void ShowStatusInfoHandler::handle(InPacket& recv) const
{
    std::int8_t mode = recv.read_byte();
    if (mode == 0) {
        std::int8_t mode2 = recv.read_byte();
        if (mode2 == 0) {
            std::int32_t itemid = recv.read_int();
            std::int32_t qty = recv.read_int();

            const ItemData& idata = ItemData::get(itemid);
            if (!idata.is_valid()) {
                return;
            }

            show_status(Text::WHITE,
                        str::concat("Gained an item: ",
                                    idata.get_name(),
                                    " (",
                                    qty < 0 ? '-' : '+',
                                    std::to_string(qty),
                                    ')'));
        } else if (mode2 == 1) {
            recv.skip(1);

            std::int32_t gain = recv.read_int();

            show_status(Text::WHITE,
                        str::concat("Received mesos (",
                                    gain < 0 ? '-' : '+',
                                    std::to_string(gain),
                                    ')'));
        }
    } else if (mode == 3) {
        bool white = recv.read_bool();
        std::int32_t gain = recv.read_int();
        bool inchat = recv.read_bool();
        std::int32_t bonus1 = recv.read_int();

        recv.read_short();
        recv.read_int();  // bonus 2
        recv.read_bool(); // 'event or party'
        recv.read_int();  // bonus 3
        recv.read_int();  // bonus 4
        recv.read_int();  // bonus 5

        if (inchat) {
            // TODO ?
        } else {
            show_status(white ? Text::WHITE : Text::YELLOW,
                        str::concat("You have gained experience (+",
                                    std::to_string(gain),
                                    ')'));
            if (bonus1 > 0) {
                show_status(Text::YELLOW,
                            str::concat("+ Bonus EXP (+",
                                        std::to_string(bonus1),
                                        ')'));
            }
        }
    } else if (mode == 4) {
        std::int32_t gain = recv.read_int();

        show_status(Text::WHITE,
                    str::concat("Received fame (",
                                gain < 0 ? '-' : '+',
                                std::to_string(gain),
                                ')'));
    } else if (mode == 5) {
        // TODO ?
    }
}

void ShowStatusInfoHandler::show_status(Text::Color color,
                                        std::string&& message) const
{
    if (auto messenger = UI::get().get_element<UIStatusMessenger>()) {
        messenger->show_status(color, std::move(message));
    }
}

void ServerMessageHandler::handle(InPacket& recv) const
{
    std::int8_t type = recv.read_byte();
    bool servermessage = recv.inspect_bool();
    if (servermessage) {
        recv.skip(1);
    }
    std::string message = recv.read_string();

    if (type == 3) {
        recv.read_byte(); // channel
        recv.read_bool(); // megaphone
    } else if (type == 4) {
        UI::get().set_scroll_notice(std::move(message));
    } else if (type == 7) {
        recv.read_int(); // npc_id
    }
}

void WeekEventMessageHandler::handle(InPacket& recv) const
{
    recv.read_byte(); // always 0xFF in solaxia and moople
    std::string message = recv.read_string();

    static constexpr const std::string_view MAPLETIP = "[MapleTip]";
    if (std::string_view(message)
            .substr(0, MAPLETIP.length())
            .compare(MAPLETIP)) {
        message.insert(0, "[Notice] ", 9);
    }

    UI::get().get_element<UIStatusbar>()->send_chatline(std::move(message),
                                                        UIChatbar::YELLOW);
}

void ChatReceivedHandler::handle(InPacket& recv) const
{
    std::int32_t charid = recv.read_int();
    recv.read_bool(); // 'gm'
    std::string message = recv.read_string();
    std::int8_t type = recv.read_byte();

    if (auto character = Stage::get().get_character(charid)) {
        auto char_name = character->get_name();
        message.reserve(message.capacity() + char_name.length() + 2);
        message.insert(0, char_name);
        message.insert(char_name.length(), ": ");
        character->speak(std::string{message});
    }

    auto linetype = static_cast<UIChatbar::LineType>(type);
    if (auto statusbar = UI::get().get_element<UIStatusbar>()) {
        statusbar->send_chatline(std::move(message), linetype);
    }
}

void ScrollResultHandler::handle(InPacket& recv) const
{
    std::int32_t cid = recv.read_int();
    bool success = recv.read_bool();
    bool destroyed = recv.read_bool();
    recv.read_short(); // Legendary Spirit, if `1`.

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
        if (auto statusbar = UI::get().get_element<UIStatusbar>()) {
            statusbar->display_message(message, UIChatbar::RED);
        }

        UI::get().enable();
    }
}

void ShowItemGainInChatHandler::handle(InPacket& recv) const
{
    switch (recv.read_byte()) {
    case 3: {
        std::int8_t mode = recv.read_byte();
        if (mode == 1) { // This is _actually_ "item gain in chat".
            std::int32_t itemid = recv.read_int();
            std::int32_t qty = recv.read_int();

            const ItemData& idata = ItemData::get(itemid);
            if (!idata.is_valid()) {
                return;
            }

            if (auto statusbar = UI::get().get_element<UIStatusbar>()) {
                statusbar->send_chatline(str::concat("Gained an item: ",
                                                     idata.get_name(),
                                                     " (",
                                                     qty < 0 ? '-' : '+',
                                                     std::to_string(qty),
                                                     ')'),
                                         UIChatbar::BLUE);
            }
        } else {
            std::cout << "ShowItemGainInChatHandler: mode != 1\n"
                      << std::flush;
        }
        break;
    }
    case 4: {             // pet levelup
        recv.read_byte(); // dummy byte
        [[maybe_unused]] auto pet_index
            = static_cast<std::size_t>(recv.read_byte());
        std::cout << "ShowItemGainInChatHandler: pet levelup\n" << std::flush;
        break;
    }
    case 7: // portal sound
        std::cout << "ShowItemGainInChatHandler: portal sound\n" << std::flush;
        break;
    case 9: // quest complete effect
        std::cout << "ShowItemGainInChatHandler: quest complete effect\n"
                  << std::flush;
        break;
    case 10: { // "show own recovery", used for e.g. chairs
        [[maybe_unused]] std::int8_t amount = recv.read_byte();
        std::cout << "ShowItemGainInChatHandler: show own recovery\n"
                  << std::flush;
        break;
    }
    case 13: // card effect/"show gain card"
        Stage::get().get_player().show_effect_id(CharEffect::MONSTER_CARD);
        break;
    case 14: // monster book pickup effect
        std::cout << "ShowItemGainInChatHandler: monster book pickup effect\n"
                  << std::flush;
        break;
    case 15: // item/equip levelup effect
        std::cout << "ShowItemGainInChatHandler: item/equip levelup effect\n"
                  << std::flush;
        break;
    case 16: { // maker effect
        [[maybe_unused]] bool success = recv.read_int() == 0;
        std::cout << "ShowItemGainInChatHandler: maker effect\n" << std::flush;
        break;
    }
    case 18:                // intro effect
        recv.read_string(); // path
        std::cout << "ShowItemGainInChatHandler: intro effect\n" << std::flush;
        break;
    case 21: { // "show wheels left"
        [[maybe_unused]] auto wheels_left = recv.read_byte();
        std::cout << "ShowItemGainInChatHandler: show wheels left\n"
                  << std::flush;
        break;
    }
    case 23:                // show info
        recv.read_string(); // path
        recv.read_int();    // dummy int
        std::cout << "ShowItemGainInChatHandler: show info\n" << std::flush;
        break;
    default: { // buff effect/"show own buff effect"
        std::int32_t skillid = recv.read_int();
        // More bytes, but we don't need them.
        Stage::get().get_combat().show_player_buff(skillid);
        break;
    }
    }
}
} // namespace jrc
