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
#pragma once
#include "../../Gameplay/Combat/Attack.h"
#include "../OutPacket.h"

namespace jrc
{
//! Notifies the server of an attack. The opcode is determined by the attack
//! type. Attack::CLOSE = CLOSE_ATTACK(44) Attack::RANGED = RANGED_ATTACK(45)
//! Attack::MAGIC = MAGIC_ATTACK(46)
class AttackPacket : public OutPacket
{
public:
    AttackPacket(const AttackResult& attack)
        : OutPacket(opcodefor(attack.type))
    {
        skip(1);

        write_byte((attack.mob_count << 4) | attack.hit_count);
        write_int(attack.skill);
        if (attack.charge > 0) {
            write_int(attack.charge);
        }

        skip(8);

        write_byte(attack.display);
        write_byte(attack.to_left);
        write_byte(attack.stance);

        skip(1);

        write_byte(attack.speed);

        if (attack.type == Attack::RANGED) {
            skip(1);
            write_byte(attack.to_left);
            skip(7);
            // skip(4); if hurricane, piercing arrow or rapidfire
        } else {
            skip(4);
        }

        for (auto& damagetomob : attack.damage_lines) {
            write_int(damagetomob.first);

            skip(14);

            for (auto& singledamage : damagetomob.second) {
                write_int(singledamage.first);
                // add critical here
            }

            if (attack.skill != 5221004) {
                skip(4);
            }
        }
    }

private:
    static OutPacket::Opcode opcodefor(Attack::Type type)
    {
        switch (type) {
        case Attack::CLOSE:
            return CLOSE_ATTACK;
        case Attack::RANGED:
            return RANGED_ATTACK;
        default:
            return MAGIC_ATTACK;
        }
    }
};

//! Tells the server that the player took damage.
//! Opcode: TAKE_DAMAGE(48)
class TakeDamagePacket : public OutPacket
{
public:
    enum From : std::int8_t { TOUCH = -1 };

    TakeDamagePacket(std::int8_t from,
                     std::uint8_t element,
                     std::int32_t damage,
                     std::int32_t mobid,
                     std::int32_t oid,
                     std::uint8_t direction)
        : OutPacket(TAKE_DAMAGE)
    {
        write_time();
        write_byte(from);
        write_byte(element);
        write_int(damage);
        write_int(mobid);
        write_int(oid);
        write_byte(direction);
    }

    //! From mob attack result.
    TakeDamagePacket(const MobAttackResult& result, From from)
        : TakeDamagePacket{from,
                           0,
                           result.damage,
                           result.mob_id,
                           result.oid,
                           result.direction}
    {
    }
};

//! Packet which notifies the server of a skill usage.
//! Opcode: USE_SKILL(91)
class UseSkillPacket : public OutPacket
{
public:
    UseSkillPacket(std::int32_t skillid, std::int32_t level)
        : OutPacket(USE_SKILL)
    {
        write_time();
        write_int(skillid);
        write_byte(static_cast<std::uint8_t>(level));

        // if monster magnet : some more bytes

        if (skillid % 10000000 == 1004) {
            skip(2); // no idea what this could be
        }

        // a point (4 bytes) could be added at the end
    }
};
} // namespace jrc
