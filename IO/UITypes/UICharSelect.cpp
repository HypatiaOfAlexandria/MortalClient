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
#include "UICharSelect.h"

#include "../../Audio/Audio.h"
#include "../../Character/Job.h"
#include "../../Configuration.h"
#include "../../Constants.h"
#include "../../Net/Packets/SelectCharPackets.h"
#include "../Components/AreaButton.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "UICharCreation.h"
#include "UISoftKey.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UICharSelect::UICharSelect(std::vector<CharEntry> cs,
                           std::uint8_t c,
                           std::uint8_t s,
                           std::uint8_t channel_id,
                           std::int8_t p)
    : characters(cs), require_pic(p), charcount_absolute(c), slots_absolute(s)
{
    selected_absolute = Setting<DefaultCharacter>::get().load();
    selected_relative = selected_absolute % PAGESIZE;
    page = selected_absolute / PAGESIZE;

    nl::node title = nl::nx::ui["Login.img"]["Title"];
    nl::node common = nl::nx::ui["Login.img"]["Common"];
    nl::node charselect = nl::nx::ui["Login.img"]["CharSelect"];

    sprites.emplace_back(title["worldsel"]);
    sprites.emplace_back(common["frame"], Point<std::int16_t>(400, 290));

    // Post BB
    /*
    selworldpos = Point<std::int16_t>(578, 42);
    charinfopos = Point<std::int16_t>(662, 355);
    buttons[BT_ARBEIT] = std::make_unique<MapleButton>(charsel["arbeit"],
    Point<std::int16_t>(580, 115))); buttons[BT_CARDS] =
    std::make_unique<MapleButton>(charsel["characterCard"],
    Point<std::int16_t>(665, 115)));
    */

    // Pre BB
    charinfopos = Point<std::int16_t>(662, 305);
    selworldpos = Point<std::int16_t>(578, 112);

    sprites.emplace_back(charselect["charInfo"], charinfopos);
    sprites.emplace_back(common["selectWorld"], selworldpos);
    sprites.emplace_back(charselect["selectedWorld"]["icon"]["15"],
                         selworldpos);
    sprites.emplace_back(charselect["selectedWorld"]["name"]["15"],
                         selworldpos);
    sprites.emplace_back(charselect["selectedWorld"]["ch"][channel_id],
                         selworldpos);

    emptyslot = charselect["buyCharacter"];
    nametag = charselect["nameTag"];

    buttons[BT_SELECTCHAR] = std::make_unique<MapleButton>(
        charselect["BtSelect"], charinfopos + Point<std::int16_t>(-76, 72));
    buttons[BT_CREATECHAR] = std::make_unique<MapleButton>(
        charselect["BtNew"], Point<std::int16_t>(200, 495));
    buttons[BT_DELETECHAR] = std::make_unique<MapleButton>(
        charselect["BtDelete"], Point<std::int16_t>(320, 495));
    buttons[BT_PAGELEFT] = std::make_unique<MapleButton>(
        charselect["pageL"], Point<std::int16_t>(100, 490));
    buttons[BT_PAGERIGHT] = std::make_unique<MapleButton>(
        charselect["pageR"], Point<std::int16_t>(490, 490));

    for (std::uint8_t i = 0; i < PAGESIZE; ++i) {
        buttons[BT_CHAR0 + i] = std::make_unique<AreaButton>(
            Point<std::int16_t>(105 + (120 * (i % 4)), 170 + (200 * (i > 3))),
            Point<std::int16_t>(50, 80));
    }

    levelset = {charselect["lv"], Charset::CENTER};

    namelabel = {Text::A18M, Text::CENTER};
    for (std::size_t i = 0; i < NUM_LABELS; ++i) {
        infolabels[i] = {Text::A11M, Text::RIGHT};
    }

    for (const auto& entry : characters) {
        charlooks.emplace_back(entry.look);
        nametags.emplace_back(
            nametag, Text::A13M, Text::WHITE, entry.stats.name);
    }

    update_counts();
    update_selection();

    position = {0, 0};
    dimension = {Constants::VIEWWIDTH, Constants::VIEWHEIGHT};
    active = true;
}

void UICharSelect::draw(float alpha) const
{
    UIElement::draw(alpha);

    for (std::uint8_t i = 0; i < charcount_relative; ++i) {
        Point<std::int16_t> charpos = get_char_pos(i);
        std::uint8_t index = i + page * PAGESIZE;
        charlooks[index].draw(charpos, alpha);
        nametags[index].draw(charpos);
    }

    if (selected_relative < charcount_relative) {
        const StatsEntry& stats = characters[selected_relative].stats;

        std::string levelstr = std::to_string(stats.stats[Maplestat::LEVEL]);
        std::int16_t lvx = levelset.draw(
            levelstr, charinfopos + Point<std::int16_t>(23, -93));
        levelset.draw('l',
                      charinfopos + Point<std::int16_t>(-7 - lvx / 2, -93));

        namelabel.draw(charinfopos + Point<std::int16_t>(0, -85));

        for (std::size_t i = 0; i < NUM_LABELS; ++i) {
            Point<std::int16_t> labelpos = charinfopos + get_label_pos(i);
            infolabels[i].draw(labelpos);
        }
    }

    for (std::uint8_t i = charcount_relative; i < slots_relative; ++i) {
        Point<std::int16_t> position_slot(130 + (120 * (i % 4)),
                                          250 + (200 * (i > 3)));
        emptyslot.draw(position_slot, alpha);
    }
}

void UICharSelect::update()
{
    UIElement::update();

    emptyslot.update();

    for (auto& chit : charlooks) {
        chit.update(Constants::TIMESTEP);
    }
}

Button::State UICharSelect::button_pressed(std::uint16_t bid)
{
    if (bid >= BT_CHAR0) {
        nametags[selected_absolute].set_selected(false);
        charlooks[selected_absolute].set_stance(Stance::STAND1);
        buttons[BT_CHAR0 + selected_relative]->set_state(Button::NORMAL);

        selected_relative = static_cast<std::uint8_t>(bid - BT_CHAR0);
        selected_absolute = selected_relative + page * PAGESIZE;
        update_selection();

        return Button::IDENTITY;
    } else {
        switch (bid) {
        case BT_SELECTCHAR:
            send_selection();
            return Button::NORMAL;
        case BT_CREATECHAR:
            active = false;
            UI::get().emplace<UICharcreation>();
            return Button::NORMAL;
        case BT_DELETECHAR:
            send_deletion();
            return Button::PRESSED;
        case BT_PAGERIGHT:
            page++;
            update_counts();
            update_selection();
            return Button::IDENTITY;
        case BT_PAGELEFT:
            page--;
            update_counts();
            update_selection();
            return Button::IDENTITY;
        default:
            return Button::PRESSED;
        }
    }
}

void UICharSelect::update_selection()
{
    if (selected_relative >= charcount_relative) {
        return;
    }

    charlooks[selected_absolute].set_stance(Stance::WALK1);
    nametags[selected_absolute].set_selected(true);

    buttons[BT_CHAR0 + selected_relative]->set_state(Button::PRESSED);
    namelabel.change_text(characters[selected_relative].stats.name);

    for (std::size_t i = 0; i < NUM_LABELS; ++i) {
        infolabels[i].change_text(get_label_string(i));
    }
}

void UICharSelect::update_counts()
{
    if (page > 0) {
        buttons[BT_PAGELEFT]->set_state(Button::NORMAL);
    } else {
        buttons[BT_PAGELEFT]->set_state(Button::DISABLED);
    }

    if (page < slots_absolute / PAGESIZE) {
        buttons[BT_PAGERIGHT]->set_state(Button::NORMAL);
    } else {
        buttons[BT_PAGERIGHT]->set_state(Button::DISABLED);
    }

    charcount_relative = charcount_absolute;
    if (charcount_relative > (page + 1) * PAGESIZE) {
        charcount_relative = PAGESIZE;
    } else if (charcount_relative < page * PAGESIZE) {
        charcount_relative = 0;
    } else {
        charcount_relative -= page * PAGESIZE;
    }

    if (selected_absolute >= charcount_absolute) {
        selected_absolute = 0;
    }

    selected_relative = selected_absolute % PAGESIZE;
    if (selected_relative >= charcount_relative) {
        selected_relative = 0;
    }

    slots_relative = slots_absolute - page * PAGESIZE;
    if (slots_relative > PAGESIZE) {
        slots_relative = PAGESIZE;
    }

    if (charcount_relative < slots_relative) {
        buttons[BT_CREATECHAR]->set_state(Button::NORMAL);
    } else {
        buttons[BT_CREATECHAR]->set_state(Button::DISABLED);
    }

    if (charcount_relative > 0) {
        buttons[BT_DELETECHAR]->set_state(Button::NORMAL);
    } else {
        buttons[BT_DELETECHAR]->set_state(Button::DISABLED);
    }

    for (std::uint8_t i = 0; i < PAGESIZE; ++i) {
        if (i < charcount_relative) {
            buttons[BT_CHAR0 + i]->set_state(Button::NORMAL);
        } else {
            buttons[BT_CHAR0 + i]->set_state(Button::DISABLED);
        }
    }
}

void UICharSelect::send_selection()
{
    if (selected_relative >= charcount_relative) {
        buttons[BT_SELECTCHAR]->set_state(Button::MOUSEOVER);
        return;
    }

    Setting<DefaultCharacter>::get().save(selected_absolute);
    std::int32_t cid = characters[selected_absolute].cid;
    switch (require_pic) {
    case 0:
        UI::get().emplace<UISoftkey>([cid](const std::string& pic) {
            UI::get().disable();
            RegisterPicPacket(cid, pic).dispatch();
        });
        break;
    case 1:
        UI::get().emplace<UISoftkey>([cid](const std::string& pic) {
            UI::get().disable();
            SelectCharPicPacket(pic, cid).dispatch();
        });
        break;
    case 2:
        UI::get().disable();
        Sound(Sound::SELECTCHAR).play();
        SelectCharPacket(cid).dispatch();
        break;
    default:
        Console::get().print("require_pic = " + std::to_string(require_pic));
        break;
    }
}

void UICharSelect::send_deletion()
{
    if (selected_relative >= charcount_relative) {
        buttons[BT_DELETECHAR]->set_state(Button::MOUSEOVER);
        return;
    }

    std::int32_t cid = characters[selected_absolute].cid;
    UI::get().emplace<UISoftkey>([cid](const std::string& pic) {
        UI::get().disable();
        DeleteCharPacket(pic, cid).dispatch();
    });
}

void UICharSelect::add_character(CharEntry&& character)
{
    characters.emplace_back(std::forward<CharEntry>(character));
    charlooks.emplace_back(character.look);
    nametags.emplace_back(
        nametag, Text::A13M, Text::WHITE, character.stats.name);
    charcount_absolute++;
    charcount_relative++;

    update_counts();
    update_selection();
}

void UICharSelect::remove_char(std::int32_t cid)
{
    std::size_t index = 0;
    for (const auto& character : characters) {
        if (character.cid == cid) {
            break;
        }

        index++;
    }

    if (index == characters.size()) {
        return;
    }

    characters.erase(characters.begin() + index);
    charlooks.erase(charlooks.begin() + index);
    nametags.erase(nametags.begin() + index);

    charcount_absolute--;
    charcount_relative--;
    update_counts();
    update_selection();
}

const CharEntry& UICharSelect::get_character(std::int32_t cid)
{
    for (const auto& character : characters) {
        if (character.cid == cid) {
            return character;
        }
    }

    Console::get().print(__func__,
                         "Warning: Invalid cid (" + std::to_string(cid) + ")");

    static const CharEntry null_entry{{}, {}, 0};
    return null_entry;
}

std::string UICharSelect::get_label_string(std::size_t label) const
{
    const StatsEntry& stats = characters[selected_relative].stats;
    switch (label) {
    case JOB:
        return Job(stats.stats[Maplestat::JOB]).get_name();
    case WORLDRANK:
        return std::to_string(stats.rank.first);
    case JOBRANK:
        return std::to_string(stats.jobrank.first);
    case STR:
        return std::to_string(stats.stats[Maplestat::STR]);
    case DEX:
        return std::to_string(stats.stats[Maplestat::DEX]);
    case INT:
        return std::to_string(stats.stats[Maplestat::INT]);
    case LUK:
        return std::to_string(stats.stats[Maplestat::LUK]);
    default:
        return "";
    }
}

Point<std::int16_t> UICharSelect::get_label_pos(std::size_t label) const
{
    switch (label) {
    case JOB:
        return {72, -48};
    case WORLDRANK:
        return {72, -24};
    case JOBRANK:
        return {72, -4};
    case STR:
        return {-5, 26};
    case DEX:
        return {-5, 48};
    case INT:
        return {72, 26};
    case LUK:
        return {72, 48};
    default:
        return {};
    }
}

Point<std::int16_t> UICharSelect::get_char_pos(std::size_t i) const
{
    std::int16_t x = 130 + (120 * (i % 4));
    std::int16_t y = 250 + (200 * (i > 3));
    return {x, y};
}
} // namespace jrc
