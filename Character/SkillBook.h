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
#include <cstdint>
#include <map>
#include <unordered_map>

namespace jrc
{
// Class that stores all information about the skills of an individual
// character.
class Skillbook
{
public:
    void set_skill(int32_t id,
                   int32_t level,
                   int32_t masterlevel,
                   int64_t expiration);

    bool has_skill(int32_t id) const;
    int32_t get_level(int32_t id) const;
    int32_t get_masterlevel(int32_t id) const;
    int64_t get_expiration(int32_t id) const;

    // Return id and level of all passive skills.
    // An ordered map is used so that lower passive skills don't override
    // higher ones.
    std::map<int32_t, int32_t> collect_passives() const;

private:
    struct SkillEntry {
        int32_t level;
        int32_t masterlevel;
        int64_t expiration;
    };

    std::unordered_map<int32_t, SkillEntry> skillentries;
};
} // namespace jrc
