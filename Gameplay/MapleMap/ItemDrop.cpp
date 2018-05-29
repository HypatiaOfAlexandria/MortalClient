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
#include "ItemDrop.h"

namespace jrc
{
ItemDrop::ItemDrop(std::int32_t oid,
                   std::int32_t owner,
                   Point<std::int16_t> start,
                   Point<std::int16_t> dest,
                   std::int8_t type,
                   std::int8_t mode,
                   std::int32_t iid,
                   bool pd,
                   const Texture& icn)
    : Drop(oid, owner, start, dest, type, mode, pd), icon(icn), itemid(iid)
{
}

void ItemDrop::draw(double viewx, double viewy, float alpha) const
{
    if (!active)
        return;

    Point<std::int16_t> absp = phobj.get_absolute(viewx, viewy, alpha);
    icon.draw({angle.get(alpha), absp, opacity.get(alpha)});
}
} // namespace jrc
