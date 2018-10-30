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
#include "ItemTooltip.h"

#include "../../Data/ItemData.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
ItemTooltip::ItemTooltip()
{
    nl::node item_tt = nl::nx::ui["UIToolTip.img"]["Item"];

    top = item_tt["Frame"]["top"];
    mid = item_tt["Frame"]["line"];
    line = item_tt["Frame"]["dotline"];
    bot = item_tt["Frame"]["bottom"];
    base = item_tt["ItemIcon"]["base"];
    cover = item_tt["ItemIcon"]["cover"];
    shade = item_tt["ItemIcon"]["shade"];

    itemid = 0;
}

bool ItemTooltip::set_item(std::int32_t iid)
{
    if (itemid == iid) {
        return false;
    }

    itemid = iid;

    if (itemid == 0) {
        return false;
    }

    const ItemData& idata = ItemData::get(itemid);

    itemicon = idata.get_icon(false);
    name = {Text::A12B,
            Text::CENTER,
            Text::WHITE,
            std::string{idata.get_name()},
            240};
    desc = {Text::A12M,
            Text::LEFT,
            Text::WHITE,
            std::string{idata.get_desc()},
            150};

    fill_length = 81 + name.height();
    std::int16_t descdelta = desc.height() - 80;
    if (descdelta > 0) {
        fill_length += descdelta;
    }

    return true;
}

void ItemTooltip::draw(Point<std::int16_t> pos) const
{
    if (itemid == 0) {
        return;
    }

    top.draw(pos);
    mid.draw({pos + Point<std::int16_t>{0, 13},
              Point<std::int16_t>{0, fill_length}});
    bot.draw(pos + Point<std::int16_t>{0, fill_length + 13});

    name.draw(pos + Point<std::int16_t>{130, 3});

    pos.shift_y(4 + name.height());

    base.draw(pos + Point<std::int16_t>{10, 10});
    shade.draw(pos + Point<std::int16_t>{10, 10});
    itemicon.draw({pos + Point<std::int16_t>{20, 82}, 2.0f, 2.0f});
    cover.draw(pos + Point<std::int16_t>{10, 10});

    desc.draw(pos + Point<std::int16_t>{100, 6});
}
} // namespace jrc
