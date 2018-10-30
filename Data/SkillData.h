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
#include "../Character/Inventory/Weapon.h"
#include "../Graphics/Texture.h"
#include "../Template/Cache.h"
#include "../Template/Rectangle.h"

#include <array>
#include <string>
#include <unordered_map>

namespace jrc
{
//! Contains information about a skill.
class SkillData : public Cache<SkillData>
{
public:
    //! The stats of one level.
    struct Stats {
        float damage;
        std::int32_t matk;
        std::int32_t fixdamage;
        std::int32_t mastery;
        std::uint8_t attackcount;
        std::uint8_t mobcount;
        std::uint8_t bulletcount;
        std::int16_t bulletcost;
        std::int32_t hpcost;
        std::int32_t mpcost;
        float chance;
        float critical;
        float ignoredef;
        float hrange;
        Rectangle<std::int16_t> range;

        constexpr Stats(float damage,
                        std::int32_t matk,
                        std::int32_t fixdamage,
                        std::int32_t mastery,
                        std::uint8_t attackcount,
                        std::uint8_t mobcount,
                        std::uint8_t bulletcount,
                        std::int16_t bulletcost,
                        std::int32_t hpcost,
                        std::int32_t mpcost,
                        float chance,
                        float critical,
                        float ignoredef,
                        float hrange,
                        Rectangle<std::int16_t> range) noexcept
            : damage(damage),
              matk(matk),
              fixdamage(fixdamage),
              mastery(mastery),
              attackcount(attackcount),
              mobcount(mobcount),
              bulletcount(bulletcount),
              bulletcost(bulletcost),
              hpcost(hpcost),
              mpcost(mpcost),
              chance(chance),
              critical(critical),
              ignoredef(ignoredef),
              hrange(hrange),
              range(range)
        {
        }
    };

    //! Skill flags, unfortunately these just have to be hardcoded
    enum Flags { NONE = 0x0000, ATTACK = 0x0001, RANGED = 0x0002 };

    //! Icon types
    enum Icon { NORMAL, DISABLED, MOUSEOVER, NUM_ICONS };

    //! Return whether the skill is passive.
    [[nodiscard]] bool is_passive() const noexcept;
    //! Return whether the skill is an attack skill.
    [[nodiscard]] bool is_attack() const noexcept;
    //! Return whether this skill is invisible in the skill book ui.
    [[nodiscard]] bool is_invisible() const noexcept;
    //! Return the default masterlevel.
    [[nodiscard]] std::int32_t get_master_level() const noexcept;
    //! Return the required weapon.
    [[nodiscard]] Weapon::Type get_required_weapon() const noexcept;
    //! Return the stats of one level.
    //!
    //! If there are no stats for that level, a default object is returned.
    [[nodiscard]] const Stats& get_stats(std::int32_t level) const noexcept;

    //! Return the name of the skill.
    [[nodiscard]] std::string_view get_name() const noexcept;
    //! Return the description of the skill.
    [[nodiscard]] std::string_view get_desc() const noexcept;
    //! Return the description of a level.
    //!
    //! If there is no description for this level, a warning message is
    //! returned.
    [[nodiscard]] std::string_view get_level_desc(std::int32_t level) const
        noexcept;

    //! Return one of the skill icons.
    //!
    //! Cannot fail if type is a valid enum.
    [[nodiscard]] const Texture& get_icon(Icon icon) const noexcept;

private:
    //! Allow the cache to use the constructor.
    friend Cache<SkillData>;
    //! Load a skill from the game files.
    SkillData(std::int32_t id);

    //! Get some hardcoded information.
    [[nodiscard]] std::int32_t flags_of(std::int32_t id) const noexcept;

    std::unordered_map<std::int32_t, Stats> stats;
    std::string element;
    Weapon::Type reqweapon;
    std::int32_t masterlevel;
    std::int32_t flags;
    bool passive;
    bool invisible;

    std::string name;
    std::string desc;
    std::unordered_map<std::int32_t, std::string> levels;

    std::array<Texture, NUM_ICONS> icons;
};
} // namespace jrc
