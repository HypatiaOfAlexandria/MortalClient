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
#include "Buff.h"

namespace jrc
{
namespace Buffstat
{
const std::unordered_map<Id, std::uint64_t> first_codes
    = {{DASH2, 0x8000000000000L},
       {DASH, 0x10000000000000L},
       {ELEMENTAL_RESET, 0x200000000L},
       {ARAN_COMBO, 0x1000000000L},
       {COMBO_DRAIN, 0x2000000000L},
       {COMBO_BARRIER, 0x4000000000L},
       {BODY_PRESSURE, 0x8000000000L},
       {SMART_KNOCKBACK, 0x10000000000L},
       {PYRAMID_PQ, 0x20000000000L},
       {ENERGY_CHARGE, 0x4000000000000L},
       {MONSTER_RIDING, 0x20000000000000L},
       {HOMING_BEACON, 0x80000000000000L},
       {SPEED_INFUSION, 0x100000000000000L}};

const std::unordered_map<Id, std::uint64_t> second_codes
    = {{NONE, 0x0},
       {MORPH, 0x2},
       {RECOVERY, 0x4},
       {MAPLE_WARRIOR, 0x8},
       {STANCE, 0x10},
       {SHARP_EYES, 0x20},
       {MANA_REFLECTION, 0x40},
       {SHADOW_CLAW, 0x100},
       {INFINITY_, 0x200},
       {HOLY_SHIELD, 0x400},
       {HAMSTRING, 0x800},
       {BLIND, 0x1000},
       {CONCENTRATE, 0x2000},
       {ECHO_OF_HERO, 0x8000},
       {GHOST_MORPH, 0x20000},
       {AURA, 0x40000},
       {CONFUSE, 0x80000},
       {BERSERK_FURY, 0x8000000},
       {DIVINE_BODY, 0x10000000},
       {SPARK, 0x20000000L},
       {FINALATTACK, 0x80000000L},
       {BATTLESHIP, 0xA00000040L},
       {WATK, 0x100000000L},
       {WDEF, 0x200000000L},
       {MATK, 0x400000000L},
       {MDEF, 0x800000000L},
       {ACC, 0x1000000000L},
       {AVOID, 0x2000000000L},
       {HANDS, 0x4000000000L},
       {SHOWDASH, 0x4000000000L},
       {SPEED, 0x8000000000L},
       {JUMP, 0x10000000000L},
       {MAGIC_GUARD, 0x20000000000L},
       {DARKSIGHT, 0x40000000000L},
       {BOOSTER, 0x80000000000L},
       {POWERGUARD, 0x100000000000L},
       {HYPERBODYHP, 0x200000000000L},
       {HYPERBODYMP, 0x400000000000L},
       {INVINCIBLE, 0x800000000000L},
       {SOULARROW, 0x1000000000000L},
       {STUN, 0x2000000000000L},
       {POISON, 0x4000000000000L},
       {SEAL, 0x8000000000000L},
       {DARKNESS, 0x10000000000000L},
       {COMBO, 0x20000000000000L},
       {SUMMON, 0x20000000000000L},
       {WK_CHARGE, 0x40000000000000L},
       {DRAGONBLOOD, 0x80000000000000L},
       {HOLY_SYMBOL, 0x100000000000000L},
       {MESOUP, 0x200000000000000L},
       {SHADOWPARTNER, 0x400000000000000L},
       {PICKPOCKET, 0x800000000000000L},
       {PUPPET, 0x800000000000000L},
       {MESOGUARD, 0x1000000000000000L},
       {WEAKEN, 0x4000000000000000L}};
} // namespace Buffstat
} // namespace jrc
