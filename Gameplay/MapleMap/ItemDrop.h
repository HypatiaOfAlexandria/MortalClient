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
#include "../../Graphics/Texture.h"
#include "Drop.h"

namespace jrc
{
class ItemDrop : public Drop
{
public:
    ItemDrop(std::int32_t oid,
             std::int32_t owner,
             Point<std::int16_t> start,
             Point<std::int16_t> dest,
             std::int8_t type,
             std::int8_t mode,
             std::int32_t iid,
             bool playerdrop,
             const Texture& icon);

    void draw(double viewx, double viewy, float alpha) const override;

private:
    const Texture& icon;
    std::int32_t itemid;
};
} // namespace jrc
