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
#include "../../Character/MapleStat.h"
#include "../OutPacket.h"

namespace jrc
{
// Requests a stat increase by spending ap.
// Opcode: SPEND_AP(87)
class SpendApPacket : public OutPacket
{
public:
    SpendApPacket(Maplestat::Id stat) : OutPacket(SPEND_AP)
    {
        write_time();
        write_int(Maplestat::codes[stat]);
    }
};

// Requests a skill level increase by spending sp.
// Opcode: SPEND_SP(90)
class SpendSpPacket : public OutPacket
{
public:
    SpendSpPacket(std::int32_t skill_id) : OutPacket(SPEND_SP)
    {
        write_time();
        write_int(skill_id);
    }
};
} // namespace jrc
