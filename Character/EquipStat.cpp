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
#include "EquipStat.h"

namespace jrc
{
Equipstat::Id Equipstat::by_id(size_t id)
{
    return static_cast<Id>(id);
}

int32_t Equipstat::value_of(Id value)
{
    return value;
}

const char* Equipstat::names[LENGTH] = {"STR",
                                        "DEX",
                                        "INT",
                                        "LUK",
                                        "MAX HP",
                                        "MAX MP",
                                        "WEAPON ATT",
                                        "MAGIC ATT",
                                        "WEAPON DEFENSE",
                                        "MAGIC DEFENSE",
                                        "ACCURACY",
                                        "AVOID",
                                        "HANDS",
                                        "SPEED",
                                        "JUMP"};
} // namespace jrc
