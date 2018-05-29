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
#include "UIStateLogin.h"

#include "UITypes/UILogin.h"

namespace jrc
{
UIStateLogin::UIStateLogin()
{
    focused = UIElement::NONE;

    emplace<UILogin>();
}

void UIStateLogin::draw(float inter, Point<std::int16_t>) const
{
    for (auto iter : elements) {
        UIElement* element = iter.second.get();
        if (element && element->is_active())
            element->draw(inter);
    }
}

void UIStateLogin::update()
{
    for (auto iter : elements) {
        UIElement* element = iter.second.get();
        if (element && element->is_active())
            element->update();
    }
}

void UIStateLogin::doubleclick(Point<std::int16_t>)
{
}

void UIStateLogin::send_key(KeyType::Id, std::int32_t, bool)
{
}

Cursor::State UIStateLogin::send_cursor(Cursor::State mst,
                                        Point<std::int16_t> pos)
{
    if (UIElement* focusedelement = get(focused)) {
        if (focusedelement->is_active()) {
            return focusedelement->send_cursor(mst == Cursor::CLICKING, pos);
        } else {
            focused = UIElement::NONE;
            return mst;
        }
    } else {
        UIElement* front = nullptr;
        UIElement::Type fronttype = UIElement::NONE;

        for (auto iter : elements) {
            auto& element = iter.second;
            if (element && element->is_active()) {
                if (element->is_in_range(pos)) {
                    if (front) {
                        element->remove_cursor(false, pos);
                    }

                    front = element.get();
                    fronttype = iter.first;
                } else {
                    element->remove_cursor(false, pos);
                }
            }
        }

        if (front) {
            return front->send_cursor(mst == Cursor::CLICKING, pos);
        } else {
            return Cursor::IDLE;
        }
    }
}

void UIStateLogin::drag_icon(Icon*)
{
}

void UIStateLogin::clear_tooltip(Tooltip::Parent)
{
}

void UIStateLogin::show_equip(Tooltip::Parent, std::int16_t)
{
}

void UIStateLogin::show_item(Tooltip::Parent, std::int32_t)
{
}

void UIStateLogin::show_skill(
    Tooltip::Parent, std::int32_t, std::int32_t, std::int32_t, std::int64_t)
{
}

template<class T, typename... Args>
void UIStateLogin::emplace(Args&&... args)
{
    if (auto iter = pre_add(T::TYPE, T::TOGGLED, T::FOCUSED)) {
        (*iter).second = std::make_unique<T>(std::forward<Args>(args)...);
    }
}

UIState::Iterator
UIStateLogin::pre_add(UIElement::Type type, bool, bool is_focused)
{
    remove(type);

    if (is_focused)
        focused = type;

    return elements.find(type);
}

void UIStateLogin::remove(UIElement::Type type)
{
    if (focused == type)
        focused = UIElement::NONE;

    if (auto& element = elements[type]) {
        element->deactivate();
        element.release();
    }
}

UIElement* UIStateLogin::get(UIElement::Type type)
{
    return elements[type].get();
}

UIElement* UIStateLogin::get_front(Point<std::int16_t> pos)
{
    auto begin = elements.values().rbegin();
    auto end = elements.values().rend();
    for (auto iter = begin; iter != end; ++iter) {
        auto& element = *iter;
        if (element && element->is_active() && element->is_in_range(pos))
            return element.get();
    }
    return nullptr;
}
} // namespace jrc
