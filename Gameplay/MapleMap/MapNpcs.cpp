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
#include "MapNpcs.h"

#include "../../Net/Packets/NpcInteractionPackets.h"
#include "Npc.h"

namespace jrc
{
void MapNpcs::draw(Layer::Id layer,
                   double viewx,
                   double viewy,
                   float alpha) const
{
    npcs.draw(layer, viewx, viewy, alpha);
}

void MapNpcs::update(const Physics& physics)
{
    for (; !spawns.empty(); spawns.pop()) {
        const NpcSpawn& spawn = spawns.front();

        std::int32_t oid = spawn.get_oid();
        nullable_ptr<MapObject> npc = npcs.get(oid);
        if (npc) {
            npc->activate();
        } else {
            npcs.add(spawn.instantiate(physics));
        }
    }

    npcs.update(physics);
}

void MapNpcs::spawn(NpcSpawn&& spawn)
{
    spawns.emplace(std::move(spawn));
}

void MapNpcs::remove(std::int32_t oid)
{
    if (auto npc = npcs.get(oid))
        npc->deactivate();
}

void MapNpcs::clear()
{
    npcs.clear();
}

Cursor::State MapNpcs::send_cursor(bool pressed,
                                   Point<std::int16_t> position,
                                   Point<std::int16_t> viewpos)
{
    for (auto& mmo : npcs) {
        Npc* npc = static_cast<Npc*>(mmo.second.get());
        if (npc && npc->is_active() && npc->in_range(position, viewpos)) {
            if (pressed) {
                // TODO: try finding dialogue first
                TalkToNPCPacket(npc->get_oid()).dispatch();
                return Cursor::IDLE;
            } else {
                return Cursor::CANCLICK;
            }
        }
    }
    return Cursor::IDLE;
}
} // namespace jrc
