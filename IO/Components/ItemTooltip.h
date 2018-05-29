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
#include "../../Graphics/Text.h"
#include "Charset.h"
#include "Tooltip.h"

namespace jrc
{
class ItemTooltip : public Tooltip
{
public:
    ItemTooltip();

    void draw(Point<std::int16_t> position) const override;

    bool set_item(std::int32_t itemid);

private:
    std::int32_t itemid;
    std::int16_t filllength;
    Texture itemicon;

    Text name;
    Text desc;
    Texture top;
    Texture mid;
    Texture line;
    Texture bot;
    Texture base;
    Texture cover;
    Texture shade;
};
} // namespace jrc
