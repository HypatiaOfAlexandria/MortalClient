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
#include "../Template/Singleton.h"
#include "../Template/nullable_ptr.h"
#include "Components/Icon.h"
#include "Components/ScrollingNotice.h"
#include "Components/Textfield.h"
#include "Cursor.h"
#include "Keyboard.h"
#include "UIState.h"

#include <unordered_map>

namespace jrc
{
class UI : public Singleton<UI>
{
public:
    enum State { LOGIN, GAME };

    UI() noexcept;

    void init();
    void draw(float alpha) const;
    void update();

    void enable();
    void disable();
    void change_state(State state);

    void quit();
    bool not_quitted() const;

    void send_cursor(Point<std::int16_t> pos);
    void send_cursor(bool pressed);
    void send_cursor(Point<std::int16_t> cursorpos, Cursor::State cursorstate);
    void doubleclick();
    void send_key(std::int32_t keycode, bool pressed);
    void send_menu(KeyAction::Id action);

    void set_scroll_notice(std::string&& notice) noexcept;
    void focus_text_field(Textfield* to_focus) noexcept;
    void drag_icon(Icon* icon);

    void
    add_keymapping(std::uint8_t no, std::uint8_t type, std::int32_t action);

    void clear_tooltip(Tooltip::Parent parent);
    void show_equip(Tooltip::Parent parent, std::int16_t slot);
    void show_item(Tooltip::Parent parent, std::int32_t item_id);
    void show_skill(Tooltip::Parent parent,
                    std::int32_t skill_id,
                    std::int32_t level,
                    std::int32_t master_level,
                    std::int64_t expiration);

    template<class T, typename... Args>
    nullable_ptr<T> emplace(Args&&... args);
    template<class T>
    nullable_ptr<T> get_element();
    void remove(UIElement::Type type);

    const Keyboard& get_keyboard() const noexcept;
    Keyboard& get_keyboard_mut() noexcept;

private:
    std::unique_ptr<UIState> state;
    Keyboard keyboard;
    Cursor cursor;
    ScrollingNotice scrolling_notice;

    nullable_ptr<Textfield> focused_text_field;
    std::unordered_map<std::int32_t, bool> is_key_down;

    bool enabled;
    bool quitted;
};

template<class T, typename... Args>
nullable_ptr<T> UI::emplace(Args&&... args)
{
    if (auto iter = state->pre_add(T::TYPE, T::TOGGLED, T::FOCUSED)) {
        (*iter).second = std::make_unique<T>(std::forward<Args>(args)...);
    }
    return state->get(T::TYPE);
}

template<class T>
nullable_ptr<T> UI::get_element()
{
    UIElement::Type type = T::TYPE;
    UIElement* element = state->get(type);
    return static_cast<T*>(element);
}
} // namespace jrc
