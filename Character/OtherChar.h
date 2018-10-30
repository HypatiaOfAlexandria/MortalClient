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
#include "../Gameplay/Movement.h"
#include "Char.h"
#include "Look/CharLook.h"

#include <queue>
#include <vector>

namespace jrc
{
//! Other client's players.
class OtherChar : public Char
{
public:
    OtherChar(std::int32_t charid,
              const CharLook& look,
              std::uint8_t level,
              std::int16_t job,
              std::string&& name,
              std::int8_t stance,
              Point<std::int16_t> position) noexcept;

    //! Update the character.
    std::int8_t update(const Physics& physics) override;
    //! Add the movements which this character will go through next.
    void send_movement(const std::vector<Movement>& movements);

    //! Update a skill level.
    void update_skill(std::int32_t skillid, std::uint8_t skilllevel);
    //! Update the attack speed.
    void update_speed(std::uint8_t attackspeed);
    //! Update the character look.
    void update_look(const LookEntry& look);

    //! Return the character's attacking speed.
    std::int8_t get_integer_attack_speed() const override;
    //! Return the character's level.
    std::uint16_t get_level() const override;
    //! Return the character's level of a skill.
    std::int32_t get_skill_level(std::int32_t skillid) const override;

private:
    std::uint16_t level;
    std::int16_t job;
    std::queue<Movement> movements;
    Movement lastmove;
    std::uint16_t timer;

    std::unordered_map<std::int32_t, std::uint8_t> skilllevels;
    std::uint8_t attackspeed;
};
} // namespace jrc
