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
#include "../../Gameplay/Physics/Physics.h"
#include "../../Graphics/Animation.h"
#include "../../Graphics/Text.h"
#include "../../Template/EnumMap.h"
#include "../../Template/Point.h"
#include "tinyutf8.hpp"

#include <cstdint>
#include <map>
#include <string>

namespace jrc
{
class PetLook
{
public:
    enum Stance : std::uint8_t {
        MOVE,
        STAND,
        JUMP,
        ALERT,
        PRONE,
        FLY,
        HANG,
        WARP,
        LENGTH
    };

    static Stance stance_by_value(std::uint8_t value)
    {
        std::uint8_t value_h = value >> 1;
        return value_h >= LENGTH ? STAND : static_cast<Stance>(value_h);
    }

    PetLook(std::int32_t iid,
            utf8_string&& name,
            std::int32_t uniqueid,
            Point<std::int16_t> pos,
            std::uint8_t stance,
            std::int32_t fhid);
    PetLook() noexcept;

    void draw(double viewx, double viewy, float alpha) const;
    void update(const Physics& physics, Point<std::int16_t> char_pos) noexcept;

    void set_position(std::int16_t xpos, std::int16_t ypos) noexcept;
    void set_stance(Stance stance) noexcept;
    void set_stance(std::uint8_t stancebyte) noexcept;

    std::int32_t get_item_id() const noexcept;
    Stance get_stance() const noexcept;

private:
    std::int32_t item_id;
    utf8_string name;
    std::int32_t unique_id;
    Stance stance;
    bool flip;

    EnumMap<Stance, Animation> animations;
    PhysicsObject phobj;
    Text namelabel;
};
} // namespace jrc
