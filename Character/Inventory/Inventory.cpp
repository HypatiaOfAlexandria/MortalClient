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
#include "Inventory.h"

#include "../../Console.h"
#include "../../Data/BulletData.h"
#include "../../Data/EquipData.h"
#include "../../Data/ItemData.h"

namespace jrc
{
Inventory::Inventory()
{
    bulletslot = 0;
    meso = 0;
    running_uid = 0;
    slotmaxima[InventoryType::EQUIPPED] = Equipslot::LENGTH;
}

void Inventory::recalc_stats(Weapon::Type type)
{
    totalstats.clear();
    for (auto& iter : inventories[InventoryType::EQUIPPED]) {
        auto equip_iter = equips.find(iter.second.unique_id);
        if (equip_iter != equips.end()) {
            const Equip& equip = equip_iter->second;
            for (auto stat_iter : totalstats) {
                stat_iter.second += equip.get_stat(stat_iter.first);
            }
        }
    }

    int32_t prefix;
    switch (type) {
    case Weapon::BOW:
        prefix = 2060;
        break;
    case Weapon::CROSSBOW:
        prefix = 2061;
        break;
    case Weapon::CLAW:
        prefix = 2070;
        break;
    case Weapon::GUN:
        prefix = 2330;
        break;
    default:
        prefix = 0;
    }

    bulletslot = 0;
    if (prefix) {
        for (auto& iter : inventories[InventoryType::USE]) {
            const Slot& slot = iter.second;
            if (slot.count && slot.item_id / 1000 == prefix) {
                bulletslot = iter.first;
                break;
            }
        }
    }

    if (int32_t bulletid = get_bulletid()) {
        totalstats[Equipstat::WATK] += BulletData::get(bulletid).get_watk();
    }
}

void Inventory::set_meso(int64_t m)
{
    meso = m;
}

void Inventory::set_slotmax(InventoryType::Id type, uint8_t slotmax)
{
    slotmaxima[type] = slotmax;
}

void Inventory::add_item(InventoryType::Id invtype,
                         int16_t slot,
                         int32_t item_id,
                         bool cash,
                         int64_t expire,
                         uint16_t count,
                         const std::string& owner,
                         int16_t flags)
{
    items.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(add_slot(invtype, slot, item_id, count, cash)),
        std::forward_as_tuple(item_id, expire, owner, flags));
}

void Inventory::add_pet(InventoryType::Id invtype,
                        int16_t slot,
                        int32_t item_id,
                        bool cash,
                        int64_t expire,
                        const std::string& name,
                        int8_t level,
                        int16_t closeness,
                        int8_t fullness)
{
    pets.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(add_slot(invtype, slot, item_id, 1, cash)),
        std::forward_as_tuple(
            item_id, expire, name, level, closeness, fullness));
}

void Inventory::add_equip(InventoryType::Id invtype,
                          int16_t slot,
                          int32_t item_id,
                          bool cash,
                          int64_t expire,
                          uint8_t slots,
                          uint8_t level,
                          const EnumMap<Equipstat::Id, uint16_t>& stats,
                          const std::string& owner,
                          int16_t flag,
                          uint8_t ilevel,
                          uint16_t iexp,
                          int32_t vicious)
{
    equips.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(add_slot(invtype, slot, item_id, 1, cash)),
        std::forward_as_tuple(item_id,
                              expire,
                              owner,
                              flag,
                              slots,
                              level,
                              stats,
                              ilevel,
                              iexp,
                              vicious));
}

void Inventory::remove(InventoryType::Id type, int16_t slot)
{
    auto iter = inventories[type].find(slot);
    if (iter == inventories[type].end())
        return;

    int32_t unique_id = iter->second.unique_id;
    inventories[type].erase(iter);

    switch (type) {
    case InventoryType::EQUIPPED:
    case InventoryType::EQUIP:
        equips.erase(unique_id);
        break;
    case InventoryType::CASH:
        items.erase(unique_id);
        pets.erase(unique_id);
        break;
    default:
        items.erase(unique_id);
        break;
    }
}

void Inventory::swap(InventoryType::Id firsttype,
                     int16_t firstslot,
                     InventoryType::Id secondtype,
                     int16_t secondslot)
{
    Slot first = std::move(inventories[firsttype][firstslot]);
    inventories[firsttype][firstslot] =
        std::move(inventories[secondtype][secondslot]);
    inventories[secondtype][secondslot] = std::move(first);

    if (!inventories[firsttype][firstslot].item_id)
        remove(firsttype, firstslot);
    if (!inventories[secondtype][secondslot].item_id)
        remove(secondtype, secondslot);
}

int32_t Inventory::add_slot(InventoryType::Id type,
                            int16_t slot,
                            int32_t item_id,
                            int16_t count,
                            bool cash)
{
    running_uid++;
    inventories[type][slot] = {running_uid, item_id, count, cash};
    return running_uid;
}

void Inventory::change_count(InventoryType::Id type,
                             int16_t slot,
                             int16_t count)
{
    auto iter = inventories[type].find(slot);
    if (iter != inventories[type].end())
        iter->second.count = count;
}

void Inventory::modify(InventoryType::Id type,
                       int16_t slot,
                       int8_t mode,
                       int16_t arg,
                       Movement move)
{
    if (slot < 0) {
        slot = -slot;
        type = InventoryType::EQUIPPED;
    }
    arg = (arg < 0) ? -arg : arg;

    switch (mode) {
    case CHANGECOUNT:
        change_count(type, slot, arg);
        break;
    case SWAP:
        switch (move) {
        case MOVE_INTERNAL:
            swap(type, slot, type, arg);
            break;
        case MOVE_UNEQUIP:
            swap(InventoryType::EQUIPPED, slot, InventoryType::EQUIP, arg);
            break;
        case MOVE_EQUIP:
            swap(InventoryType::EQUIP, slot, InventoryType::EQUIPPED, arg);
            break;
        case MOVE_NONE:
        default:
            break;
        }
        break;
    case REMOVE:
        remove(type, slot);
        break;
    default:
        break;
    }
}

uint8_t Inventory::get_slotmax(InventoryType::Id type) const
{
    return slotmaxima[type];
}

uint16_t Inventory::get_stat(Equipstat::Id type) const
{
    return totalstats[type];
}

int64_t Inventory::get_meso() const
{
    return meso;
}

bool Inventory::has_projectile() const
{
    return bulletslot > 0;
}

bool Inventory::has_equipped(Equipslot::Id slot) const
{
    return inventories[InventoryType::EQUIPPED].count(slot) > 0;
}

int16_t Inventory::get_bulletslot() const
{
    return bulletslot;
}

uint16_t Inventory::get_bulletcount() const
{
    return get_item_count(InventoryType::USE, bulletslot);
}

int32_t Inventory::get_bulletid() const
{
    return get_item_id(InventoryType::USE, bulletslot);
}

Equipslot::Id Inventory::find_equipslot(int32_t itemid) const
{
    const EquipData& cloth = EquipData::get(itemid);
    if (!cloth.is_valid())
        return Equipslot::NONE;

    Equipslot::Id eqslot = cloth.get_eqslot();
    if (eqslot == Equipslot::RING) {
        if (!has_equipped(Equipslot::RING2))
            return Equipslot::RING2;

        if (!has_equipped(Equipslot::RING3))
            return Equipslot::RING3;

        if (!has_equipped(Equipslot::RING4))
            return Equipslot::RING4;

        return Equipslot::RING;
    } else {
        return eqslot;
    }
}

int16_t Inventory::find_free_slot(InventoryType::Id type) const
{
    int16_t counter = 1;
    for (auto& iter : inventories[type]) {
        if (iter.first != counter)
            return counter;

        counter++;
    }
    return counter < slotmaxima[type] ? counter : 0;
}

int16_t Inventory::find_item(InventoryType::Id type, int32_t itemid) const
{
    for (auto& iter : inventories[type]) {
        if (iter.second.item_id == itemid)
            return iter.first;
    }
    return 0;
}

int16_t Inventory::get_item_count(InventoryType::Id type, int16_t slot) const
{
    auto iter = inventories[type].find(slot);
    if (iter != inventories[type].end()) {
        return iter->second.count;
    } else {
        return 0;
    }
}

int32_t Inventory::get_item_id(InventoryType::Id type, int16_t slot) const
{
    auto iter = inventories[type].find(slot);
    if (iter != inventories[type].end()) {
        return iter->second.item_id;
    } else {
        return 0;
    }
}

nullable_ptr<const Equip> Inventory::get_equip(InventoryType::Id type,
                                           int16_t slot) const
{
    if (type != InventoryType::EQUIPPED && type != InventoryType::EQUIP)
        return {};

    auto slot_iter = inventories[type].find(slot);
    if (slot_iter == inventories[type].end())
        return {};

    auto equip_iter = equips.find(slot_iter->second.unique_id);
    if (equip_iter == equips.end())
        return {};

    return equip_iter->second;
}

Inventory::Movement Inventory::movementbyvalue(int8_t value)
{
    if (value >= MOVE_INTERNAL && value <= MOVE_EQUIP)
        return static_cast<Movement>(value);

    Console::get().print("Unknown move type: " + std::to_string(value));
    return MOVE_NONE;
}
} // namespace jrc
