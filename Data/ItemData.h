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
#include "../Graphics/Texture.h"
#include "../Template/BoolPair.h"
#include "../Template/Cache.h"

namespace jrc
{
// Class that represents an item loaded from the game's files. Contains all
// shared data between concrete items.
class ItemData : public Cache<ItemData>
{
public:
    // Returns wether the item was loaded correctly.
    bool is_valid() const;
    // Returns wether the item was loaded correctly.
    explicit operator bool() const;

    // Returns the item id.
    int32_t get_id() const;
    // Returns the item price.
    int32_t get_price() const;
    // Returns the item's name loaded from the String.nx file.
    const std::string& get_name() const;
    // Returns the item's description loaded from the String.nx file.
    const std::string& get_desc() const;
    // Return the item category (also the node name).
    const std::string& get_category() const;
    // Returns one of the item's icons. For each item there is a 'raw' icon and
    // an icon with a drop shadow.
    const Texture& get_icon(bool raw) const;

private:
    // Allow the cache to use the constructor.
    friend Cache<ItemData>;
    // Creates an item from the game's Item.nx with the specified id.
    ItemData(int32_t itemid);

    std::string get_eqcategory(int32_t) const;

    BoolPair<Texture> icons;
    int32_t itemid;
    int32_t price;
    std::string name;
    std::string desc;
    std::string category;

    bool valid;
};
} // namespace jrc
