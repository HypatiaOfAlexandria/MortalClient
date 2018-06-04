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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "CharEquips.h"

namespace jrc
{
CharEquips::CharEquips()
{
    for (auto iter : clothes) {
        iter.second = nullptr;
    }
}

void CharEquips::draw(Equipslot::Id slot,
                      Stance::Id stance,
                      Clothing::Layer layer,
                      std::uint8_t frame,
                      const DrawArgument& args) const
{
    if (const Clothing* cloth = clothes[slot]) {
        cloth->draw(stance, layer, frame, args);
    }
}

void CharEquips::add_equip(std::int32_t itemid, const BodyDrawinfo& drawinfo)
{
    if (itemid <= 0)
        return;

    auto iter = cloth_cache.find(itemid);
    if (iter == cloth_cache.end()) {
        iter = cloth_cache
                   .emplace(std::piecewise_construct,
                            std::forward_as_tuple(itemid),
                            std::forward_as_tuple(itemid, drawinfo))
                   .first;
    }
    const Clothing& cloth = iter->second;

    Equipslot::Id slot = cloth.get_eqslot();
    clothes[slot] = &cloth;
}

void CharEquips::remove_equip(Equipslot::Id slot)
{
    clothes[slot] = nullptr;
}

bool CharEquips::is_visible(Equipslot::Id slot) const
{
    if (const Clothing* cloth = clothes[slot]) {
        return cloth->is_transparent() == false;
    } else {
        return false;
    }
}

bool CharEquips::comparelayer(Equipslot::Id slot,
                              Stance::Id stance,
                              Clothing::Layer layer) const
{
    if (const Clothing* cloth = clothes[slot]) {
        return cloth->contains_layer(stance, layer);
    } else {
        return false;
    }
}

bool CharEquips::has_overall() const
{
    return get_equip(Equipslot::TOP) / 10000 == 105;
}

bool CharEquips::has_weapon() const
{
    return get_weapon() != 0;
}

bool CharEquips::is_twohanded() const
{
    if (const Clothing* weapon = clothes[Equipslot::WEAPON]) {
        return weapon->is_twohanded();
    } else {
        return false;
    }
}

CharEquips::CapType CharEquips::getcaptype() const
{
    if (const Clothing* cap = clothes[Equipslot::CAP]) {
        const std::string& vslot = cap->get_vslot();
        if (vslot == "CpH1H5")
            return HALFCOVER;
        else if (vslot == "CpH1H5AyAs")
            return FULLCOVER;
        else if (vslot == "CpH5")
            return HEADBAND;
        else
            return NONE;
    } else {
        return NONE;
    }
}

Stance::Id CharEquips::adjust_stance(Stance::Id stance) const
{
    if (const Clothing* weapon = clothes[Equipslot::WEAPON]) {
        switch (stance) {
        case Stance::STAND1:
        case Stance::STAND2:
            return weapon->get_stand();
        case Stance::WALK1:
        case Stance::WALK2:
            return weapon->get_walk();
        default:
            return stance;
        }
    } else {
        return stance;
    }
}

std::int32_t CharEquips::get_equip(Equipslot::Id slot) const
{
    if (const Clothing* cloth = clothes[slot]) {
        return cloth->get_id();
    } else {
        return 0;
    }
}

std::int32_t CharEquips::get_weapon() const
{
    return get_equip(Equipslot::WEAPON);
}

std::unordered_map<std::int32_t, Clothing> CharEquips::cloth_cache;
} // namespace jrc
