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
#include "KeyAction.h"
#include "KeyType.h"

#include <cstdint>
#include <map>

namespace jrc
{
class Keyboard
{
public:
    struct Mapping {
        KeyType::Id type;
        int32_t action;

        Mapping() : type(KeyType::NONE), action(0)
        {
        }

        Mapping(KeyType::Id in_type, int32_t in_action)
            : type(in_type), action(in_action)
        {
        }
    };

    Keyboard();

    void assign(uint8_t key, uint8_t type, int32_t action);

    int32_t shiftcode() const;
    int32_t ctrlcode() const;
    KeyAction::Id get_ctrl_action(int32_t keycode) const;
    Mapping get_mapping(int32_t keycode) const;
    Mapping get_text_mapping(int32_t keycode, bool shift) const;

private:
    std::map<int32_t, Mapping> keymap;
    std::map<int32_t, Mapping> maplekeys;
    std::map<int32_t, KeyAction::Id> textactions;
    std::map<int32_t, bool> keystate;
};
} // namespace jrc
