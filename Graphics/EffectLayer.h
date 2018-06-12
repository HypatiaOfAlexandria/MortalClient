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
#include "../Constants.h"
#include "Sprite.h"
#include "boost/container/flat_map.hpp"

#include <vector>

namespace jrc
{
//! A list of animations. Animations will be removed after all frames were
//! displayed.
class EffectLayer
{
public:
    //! Draws all effects with a z-value strictly less than 0.
    void draw_below(Point<std::int16_t> position, float alpha) const;
    //! Draws all effects with a z-value greater than or equal to 0.
    void draw_above(Point<std::int16_t> position, float alpha) const;
    //! Remove effects whose animations are done, i.e. they have expired.
    void update();
    void add(const Animation& effect,
             const DrawArgument& args,
             std::int8_t z,
             float speed);
    void add(const Animation& effect, const DrawArgument& args, std::int8_t z);
    void add(const Animation& effect, const DrawArgument& args);
    void add(const Animation& effect);

private:
    class Effect
    {
    public:
        Effect(const Animation& a, const DrawArgument& args, float s)
            : sprite(a, args), speed(s)
        {
        }

        void draw(Point<std::int16_t> position, float alpha) const
        {
            sprite.draw(position, alpha);
        }

        bool update()
        {
            return sprite.update(
                static_cast<std::uint16_t>(Constants::TIMESTEP * speed));
        }

    private:
        Sprite sprite;
        float speed;
    };

    //! Sorted map that associates z-indices (can be negative or non-negative)
    //! with the effects at that z-index.
    boost::container::flat_map<std::int8_t, std::vector<Effect>> effects;
};
} // namespace jrc
