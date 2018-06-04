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
#pragma once
#include "../Template/EnumMap.h"
#include "../Template/nullable_ptr.h"
#include "Components/EquipTooltip.h"
#include "Components/ItemTooltip.h"
#include "Components/SkillTooltip.h"
#include "UIState.h"

#include <list>
#include <memory>

namespace jrc
{
class UIStateGame : public UIState
{
public:
    UIStateGame();

    void draw(float inter, Point<std::int16_t> cursor) const override;
    void update() override;

    void doubleclick(Point<std::int16_t> pos) override;
    void
    send_key(KeyType::Id type, std::int32_t action, bool pressed) override;
    Cursor::State send_cursor(Cursor::State mst,
                              Point<std::int16_t> pos) override;

    void drag_icon(Icon* icon) override;
    void clear_tooltip(Tooltip::Parent parent) override;
    void show_equip(Tooltip::Parent parent, std::int16_t slot) override;
    void show_item(Tooltip::Parent parent, std::int32_t itemid) override;
    void show_skill(Tooltip::Parent parent,
                    std::int32_t skill_id,
                    std::int32_t level,
                    std::int32_t masterlevel,
                    std::int64_t expiration) override;

    Iterator
    pre_add(UIElement::Type type, bool toggled, bool focused) override;
    void remove(UIElement::Type type) override;
    UIElement* get(UIElement::Type type) override;
    UIElement* get_front(Point<std::int16_t> pos) override;

private:
    void drop_icon(const Icon& icon, Point<std::int16_t> pos);
    template<class T, typename... Args>
    void emplace(Args&&... args);

    EnumMap<UIElement::Type, UIElement::UPtr, UIElement::NUM_TYPES> elements;
    std::list<UIElement::Type> elementorder;
    UIElement::Type focused;

    EquipTooltip eqtooltip;
    ItemTooltip ittooltip;
    SkillTooltip sktooltip;
    nullable_ptr<Tooltip> tooltip;
    Tooltip::Parent tooltipparent;

    nullable_ptr<Icon> draggedicon;
};
} // namespace jrc
