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
#pragma once
#include "../Graphics/Sprite.h"
#include "Components/Button.h"
#include "Components/Icon.h"
#include "Cursor.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace jrc
{
/// Base class for all types of user interfaces on screen.
class UIElement
{
public:
    using UPtr = std::unique_ptr<UIElement>;

    enum Type {
        NONE,
        LOGIN,
        LOGINWAIT,
        LOGINNOTICE,
        WORLDSELECT,
        CHARSELECT,
        CHARCREATION,
        SOFTKEYBOARD,
        STATUSMESSENGER,
        STATUSBAR,
        BUFFLIST,
        NOTICE,
        NPCTALK,
        SHOP,
        STATSINFO,
        ITEMINVENTORY,
        EQUIPINVENTORY,
        SKILLBOOK,
        NUM_TYPES
    };

    virtual ~UIElement() = default;

    virtual void draw(float inter) const;
    virtual void update();

    void makeactive();
    void deactivate();
    bool is_active() const;

    virtual void toggle_active();
    virtual Button::State button_pressed(std::uint16_t buttonid);
    virtual void send_icon(const Icon& icon, Point<std::int16_t> cursorpos);

    virtual void doubleclick(Point<std::int16_t> cursorpos);
    virtual bool is_in_range(Point<std::int16_t> cursorpos) const;
    virtual bool remove_cursor(bool clicked, Point<std::int16_t> cursorpos);
    virtual Cursor::State send_cursor(bool clicked,
                                      Point<std::int16_t> cursorpos);

protected:
    UIElement(Point<std::int16_t> position,
              Point<std::int16_t> dimension,
              bool active);
    UIElement(Point<std::int16_t> position, Point<std::int16_t> dimension);
    UIElement();

    void draw_sprites(float alpha) const;
    void draw_buttons(float alpha) const;

    std::unordered_map<std::uint16_t, std::unique_ptr<Button>> buttons;
    std::vector<Sprite> sprites;
    Point<std::int16_t> position;
    Point<std::int16_t> dimension;
    bool active;
};
} // namespace jrc
