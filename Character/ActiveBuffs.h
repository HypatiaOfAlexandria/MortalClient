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
#pragma once
#include "Buff.h"
#include "CharStats.h"

#include <memory>

namespace jrc
{
//! Interface for active buffs which are applied to character stats.
class ActiveBuff
{
public:
    virtual ~ActiveBuff() = default;

    virtual void apply_to(CharStats& stats, std::int16_t value) const = 0;
};

template<Equipstat::Id STAT>
//! Template for buffs which just add their value to a stat.
class SimpleStatBuff : public ActiveBuff
{
    void apply_to(CharStats& stats, std::int16_t value) const override;
};

template<Equipstat::Id STAT>
//! Template for buffs which apply an increase by percentage.
class PercentageStatBuff : public ActiveBuff
{
    void apply_to(CharStats& stats, std::int16_t value) const override;
};

//! Buff for MAPLEWARRIOR
class MapleWarriorBuff : public ActiveBuff
{
    void apply_to(CharStats& stats, std::int16_t value) const override;
};

//! Buff for STANCE
class StanceBuff : public ActiveBuff
{
    void apply_to(CharStats& stats, std::int16_t value) const override;
};

//! Buff for BOOSTER
class BoosterBuff : public ActiveBuff
{
    void apply_to(CharStats& stats, std::int16_t value) const override;
};

class ActiveBuffs
{
public:
    //! Register all buffs effects.
    ActiveBuffs();

    //! Return the buff effect associated with the buff stat.
    void
    apply_buff(CharStats& stats, Buffstat::Id stat, std::int16_t value) const;

private:
    EnumMap<Buffstat::Id, std::unique_ptr<ActiveBuff>> buffs;
};
} // namespace jrc
