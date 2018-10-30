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
#include "../../Template/EnumMap.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace jrc
{
namespace Stance
{
enum Id : std::uint8_t {
    NONE,
    ALERT,
    DEAD,
    FLY,
    HEAL,
    JUMP,
    LADDER,
    PRONE,
    PRONESTAB,
    ROPE,
    SHOT,
    SHOOT1,
    SHOOT2,
    SHOOTF,
    SIT,
    STABO1,
    STABO2,
    STABOF,
    STABT1,
    STABT2,
    STABTF,
    STAND1,
    STAND2,
    SWINGO1,
    SWINGO2,
    SWINGO3,
    SWINGOF,
    SWINGP1,
    SWINGP2,
    SWINGPF,
    SWINGT1,
    SWINGT2,
    SWINGT3,
    SWINGTF,
    WALK1,
    WALK2,
    LENGTH
};

Id by_state(std::int8_t state);
Id by_id(std::uint8_t id);
Id by_string(std::string_view name);

bool is_climbing(Id value);
Id base_of(Id value);
Id second_of(Id value);

extern const EnumMap<Id, std::string> names;
}; // namespace Stance
} // namespace jrc
