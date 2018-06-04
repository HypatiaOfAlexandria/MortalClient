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
#include "UIStateGame.h"

#include "../Gameplay/Stage.h"
#include "UI.h"
#include "UITypes/UIBuffList.h"
#include "UITypes/UIEquipInventory.h"
#include "UITypes/UIItemInventory.h"
#include "UITypes/UINpcTalk.h"
#include "UITypes/UIShop.h"
#include "UITypes/UISkillBook.h"
#include "UITypes/UIStatsInfo.h"
#include "UITypes/UIStatusBar.h"
#include "UITypes/UIStatusMessenger.h"
#include "Window.h"

namespace jrc
{
UIStateGame::UIStateGame()
{
    focused = UIElement::NONE;
    tooltipparent = Tooltip::NONE;

    const CharLook& look = Stage::get().get_player().get_look();
    const CharStats& stats = Stage::get().get_player().get_stats();
    const Inventory& inventory = Stage::get().get_player().get_inventory();

    emplace<UIStatusMessenger>();
    emplace<UIStatusbar>(stats);
    emplace<UIBuffList>();
    emplace<UINpcTalk>();
    emplace<UIShop>(look, inventory);
}

void UIStateGame::draw(float inter, Point<std::int16_t> cursor) const
{
    for (const auto& type : elementorder) {
        auto& element = elements[type];
        if (element && element->is_active()) {
            element->draw(inter);
        }
    }

    if (tooltip) {
        tooltip->draw(cursor);
    }

    if (draggedicon) {
        draggedicon->dragdraw(cursor);
    }
}

void UIStateGame::update()
{
    for (const auto& type : elementorder) {
        auto& element = elements[type];
        if (element && element->is_active()) {
            element->update();
        }
    }
}

void UIStateGame::drop_icon(const Icon& icon, Point<std::int16_t> pos)
{
    if (UIElement* front = get_front(pos)) {
        front->send_icon(icon, pos);
    } else {
        icon.drop_on_stage();
    }
}

void UIStateGame::doubleclick(Point<std::int16_t> pos)
{
    if (UIElement* front = get_front(pos)) {
        front->double_click(pos);
    }
}

void UIStateGame::send_key(KeyType::Id type, std::int32_t action, bool pressed)
{
    switch (type) {
    case KeyType::MENU:
        if (pressed) {
            switch (action) {
            case KeyAction::CHAR_STATS:
                emplace<UIStatsinfo>(Stage::get().get_player().get_stats());
                break;
            case KeyAction::INVENTORY:
                emplace<UIItemInventory>(
                    Stage::get().get_player().get_inventory());
                break;
            case KeyAction::EQUIPMENT_TAB:
                emplace<UIEquipInventory>(
                    Stage::get().get_player().get_inventory());
                break;
            case KeyAction::SKILL_TAB:
                emplace<UISkillbook>(Stage::get().get_player().get_stats(),
                                     Stage::get().get_player().get_skills());
                break;
            case KeyAction::KEY_CONFIG:
                // TODO
                break;
            default:
                break;
            }
        }
        break;
    case KeyType::ACTION:
    case KeyType::FACE:
    case KeyType::ITEM:
    case KeyType::SKILL:
        Stage::get().send_key(type, action, pressed);
        break;
    default:
        break;
    }
}

Cursor::State UIStateGame::send_cursor(Cursor::State mst,
                                       Point<std::int16_t> pos)
{
    if (draggedicon) {
        switch (mst) {
        case Cursor::IDLE:
            drop_icon(*draggedicon, pos);
            draggedicon->reset();
            draggedicon = {};
            return mst;
        default:
            return Cursor::GRABBING;
        }
    } else {
        bool clicked = mst == Cursor::CLICKING;
        if (UIElement* focusedelement = get(focused)) {
            if (focusedelement->is_active()) {
                return focusedelement->send_cursor(clicked, pos);
            } else {
                focused = UIElement::NONE;
                return mst;
            }
        } else {
            UIElement* front = nullptr;
            UIElement::Type fronttype = UIElement::NONE;

            for (const auto& type : elementorder) {
                auto& element = elements[type];
                if (element && element->is_active()) {
                    bool found = element->is_in_range(pos)
                                     ? true
                                     : element->remove_cursor(clicked, pos);
                    if (found) {
                        if (front) {
                            element->remove_cursor(false, pos);
                        }

                        front = element.get();
                        fronttype = type;
                    }
                }
            }

            if (Tooltip::same_ui_type(tooltipparent, fronttype)) {
                clear_tooltip(tooltipparent);
            }

            if (front) {
                if (clicked) {
                    elementorder.remove(fronttype);
                    elementorder.push_back(fronttype);
                }
                return front->send_cursor(clicked, pos);
            } else {
                return Stage::get().send_cursor(clicked, pos);
            }
        }
    }
}

void UIStateGame::drag_icon(Icon* drgic)
{
    draggedicon = drgic;
}

void UIStateGame::clear_tooltip(Tooltip::Parent parent)
{
    if (parent == tooltipparent) {
        eqtooltip.set_equip(Tooltip::NONE, 0);
        ittooltip.set_item(0);
        tooltip = {};
        tooltipparent = Tooltip::NONE;
    }
}

void UIStateGame::show_equip(Tooltip::Parent parent, std::int16_t slot)
{
    eqtooltip.set_equip(parent, slot);

    if (slot) {
        tooltip = eqtooltip;
        tooltipparent = parent;
    }
}

void UIStateGame::show_item(Tooltip::Parent parent, std::int32_t itemid)
{
    ittooltip.set_item(itemid);

    if (itemid) {
        tooltip = ittooltip;
        tooltipparent = parent;
    }
}

void UIStateGame::show_skill(Tooltip::Parent parent,
                             std::int32_t skill_id,
                             std::int32_t level,
                             std::int32_t masterlevel,
                             std::int64_t expiration)
{
    sktooltip.set_skill(skill_id, level, masterlevel, expiration);

    if (skill_id) {
        tooltip = sktooltip;
        tooltipparent = parent;
    }
}

template<class T, typename... Args>
void UIStateGame::emplace(Args&&... args)
{
    if (auto iter = pre_add(T::TYPE, T::TOGGLED, T::FOCUSED)) {
        (*iter).second = std::make_unique<T>(std::forward<Args>(args)...);
    }
}

UIState::Iterator
UIStateGame::pre_add(UIElement::Type type, bool is_toggled, bool is_focused)
{
    auto& element = elements[type];
    if (element && is_toggled) {
        elementorder.remove(type);
        elementorder.push_back(type);
        element->toggle_active();
        return elements.end();
    } else {
        remove(type);
        elementorder.push_back(type);

        if (is_focused) {
            focused = type;
        }

        return elements.find(type);
    }
}

void UIStateGame::remove(UIElement::Type type)
{
    if (type == focused) {
        focused = UIElement::NONE;
    }

    if (Tooltip::same_ui_type(tooltipparent, type)) {
        clear_tooltip(tooltipparent);
    }

    elementorder.remove(type);

    if (auto& element = elements[type]) {
        element->deactivate();
        element.release();
    }
}

UIElement* UIStateGame::get(UIElement::Type type)
{
    return elements[type].get();
}

UIElement* UIStateGame::get_front(Point<std::int16_t> pos)
{
    auto begin = elementorder.rbegin();
    auto end = elementorder.rend();
    for (auto iter = begin; iter != end; ++iter) {
        const auto& element = elements[*iter];
        if (element && element->is_active() && element->is_in_range(pos)) {
            return element.get();
        }
    }
    return nullptr;
}
} // namespace jrc
