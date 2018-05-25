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
#include "Textfield.h"

#include "../../Constants.h"
#include "../UI.h"

namespace jrc
{
Textfield::Textfield(Text::Font font,
                     Text::Alignment alignment,
                     Text::Color color,
                     Rectangle<int16_t> bnd,
                     size_t lim)
    : textlabel(font, alignment, color, "", 0, false),
      text(),
      marker(font, alignment, color, "|"),
      showmarker(true),
      markerpos(0),
      bounds(bnd),
      limit(lim),
      crypt(0),
      state(NORMAL)
{
}

Textfield::Textfield() = default;

Textfield::~Textfield() = default;

void Textfield::draw(Point<int16_t> parent) const
{
    if (state == DISABLED) {
        return;
    }

    Point<int16_t> absp = bounds.getlt() + parent;
    if (text.size() > 0) {
        textlabel.draw(absp);
    }

    if (state == FOCUSED && showmarker) {
        Point<int16_t> mpos =
            absp + Point<int16_t>(textlabel.advance(markerpos), -1);
        marker.draw(mpos);
    }
}

void Textfield::update(Point<int16_t> parent)
{
    if (state == DISABLED) {
        return;
    }

    parentpos = parent;

    elapsed += Constants::TIMESTEP;
    if (elapsed > 256) {
        showmarker = !showmarker;
        elapsed = 0;
    }
}

void Textfield::set_state(State st)
{
    if (state != st) {
        state = st;
        elapsed = 0;
        showmarker = true;

        if (state == FOCUSED) {
            UI::get().focus_textfield(this);
        }
    }
}

void Textfield::set_enter_callback(std::function<void(std::string)> on_return)
{
    onreturn = on_return;
}

void Textfield::set_key_callback(KeyAction::Id key,
                                 std::function<void(void)> action)
{
    callbacks[key] = action;
}

void Textfield::send_key(KeyType::Id type, int32_t key, bool pressed)
{
    switch (type) {
    case KeyType::ACTION:
        if (pressed) {
            switch (key) {
            case KeyAction::LEFT:
                if (markerpos > 0) {
                    markerpos--;
                }
                break;
            case KeyAction::RIGHT:
                if (markerpos < text.size()) {
                    markerpos++;
                }
                break;
            case KeyAction::BACK:
                if (text.size() > 0 && markerpos > 0) {
                    text.erase(markerpos - 1, 1);
                    markerpos--;
                    modifytext(text);
                }
                break;
            case KeyAction::RETURN:
                if (onreturn && text.size() > 0) {
                    onreturn(text);
                    text = "";
                    markerpos = 0;
                    modifytext(text);
                }
                break;
            case KeyAction::SPACE:
                if (markerpos > 0 && belowlimit()) {
                    text.insert(markerpos, 1, ' ');
                    markerpos++;
                    modifytext(text);
                }
                break;
            default:
                if (callbacks.count(key)) {
                    callbacks.at(key)();
                }
                break;
            }
        }
        break;
    case KeyType::LETTER:
    case KeyType::NUMBER:
        if (!pressed) {
            auto c = static_cast<char>(key);
            if (belowlimit()) {
                text.insert(markerpos, 1, c);
                markerpos++;
                modifytext(text);
            }
        }
        break;
    default:
        break;
    }
}

void Textfield::add_string(const std::string& str)
{
    for (char c : str) {
        if (belowlimit()) {
            text.insert(markerpos, 1, c);
            markerpos++;
            modifytext(text);
        }
    }
}

void Textfield::modifytext(const std::string& t)
{
    if (crypt > 0) {
        std::string crypted;
        crypted.insert(0, t.size(), crypt);
        textlabel.change_text(crypted);
    } else {
        textlabel.change_text(t);
    }

    text = t;
}

Cursor::State Textfield::send_cursor(Point<int16_t> cursorpos, bool clicked)
{
    if (state == DISABLED) {
        return Cursor::IDLE;
    }

    auto abs_bounds = get_bounds();
    if (abs_bounds.contains(cursorpos)) {
        if (clicked) {
            switch (state) {
            case NORMAL:
                set_state(FOCUSED);
                break;
            default:
                break;
            }
            return Cursor::CLICKING;
        } else {
            return Cursor::CANCLICK;
        }
    } else {
        if (clicked) {
            switch (state) {
            case FOCUSED:
                set_state(NORMAL);
                break;
            default:
                break;
            }
        }
        return Cursor::IDLE;
    }
}

void Textfield::change_text(const std::string& t)
{
    modifytext(t);
    markerpos = text.size();
}

void Textfield::set_cryptchar(int8_t c)
{
    crypt = c;
}

bool Textfield::belowlimit() const
{
    if (limit > 0) {
        return text.size() < limit;
    } else {
        uint16_t advance = textlabel.advance(text.size());
        return (advance + 50) < bounds.get_horizontal().length();
    }
}

const std::string& Textfield::get_text() const
{
    return text;
}

bool Textfield::empty() const
{
    return text.empty();
}

Textfield::State Textfield::get_state() const
{
    return state;
}

Rectangle<int16_t> Textfield::get_bounds() const
{
    return Rectangle<int16_t>(bounds.getlt() + parentpos,
                              bounds.getrb() + parentpos);
}
} // namespace jrc
