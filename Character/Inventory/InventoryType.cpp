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
#include "InventoryType.h"

#include "../../Console.h"

namespace jrc
{
InventoryType::Id InventoryType::by_item_id(std::int32_t item_id)
{
    constexpr Id values_by_id[6] = {NONE, EQUIP, USE, SETUP, ETC, CASH};

    std::int32_t prefix = item_id / 1000000;
    return (prefix > NONE && prefix <= CASH) ? values_by_id[prefix] : NONE;
}

InventoryType::Id InventoryType::by_value(std::int8_t value)
{
    switch (value) {
    case -1:
        return EQUIPPED;
    case 1:
        return EQUIP;
    case 2:
        return USE;
    case 3:
        return SETUP;
    case 4:
        return ETC;
    case 5:
        return CASH;
    }

    Console::get().print("Unknown inventory type: " + std::to_string(value));
    return NONE;
}
} // namespace jrc
