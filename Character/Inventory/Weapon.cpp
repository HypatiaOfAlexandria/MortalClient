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
#include "Weapon.h"

#include "../../Console.h"

namespace jrc
{
Weapon::Type Weapon::by_value(int32_t value)
{
    if (value < 130 || (value > 133 && value < 137) || value == 139 ||
        (value > 149 && value < 170) || value > 170) {
        Console::get().print("Warning: Unhandled weapon type (" +
                             std::to_string(value) + ").");
        return Weapon::NONE;
    }

    return static_cast<Type>(value);
}
} // namespace jrc
