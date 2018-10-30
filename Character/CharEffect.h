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
#include "../Template/EnumMap.h"

#include <string_view>

namespace jrc::CharEffect
{
//! Character effects from Effect.nx
enum Id {
    LEVELUP,
    JOBCHANGE,
    SCROLL_SUCCESS,
    SCROLL_FAILURE,
    MONSTER_CARD,

    LENGTH
};

constexpr const EnumMap<Id, std::string_view> PATHS{"LevelUp",
                                                    "JobChanged",
                                                    "Enchant/Success",
                                                    "Enchant/Failure",
                                                    "MonsterBook/cardGet"};
} // namespace jrc::CharEffect
