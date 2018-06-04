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
#include "../../Template/nullable_ptr.h"
#include "Portal.h"
#include "nlnx/node.hpp"

#include <unordered_map>

namespace jrc
{
// Collecton of portals on a map. Draws and updates portals.
// Also contains methods for using portals and obtaining spawn points.
class MapPortals
{
public:
    static void init();

    MapPortals(nl::node source, std::int32_t mapid);
    MapPortals();

    void update(Point<std::int16_t> playerpos);
    void draw(Point<std::int16_t> viewpos, float inter) const;

    Portal::WarpInfo find_warp_at(Point<std::int16_t> playerpos);

    Point<std::int16_t> get_portal_by_id(std::uint8_t id) const;
    Point<std::int16_t> get_portal_by_name(const std::string& name) const;

private:
    static std::unordered_map<Portal::Type, Animation> animations;

    std::unordered_map<std::uint8_t, Portal> portals_by_id;
    std::unordered_map<std::string, std::uint8_t> portal_ids_by_name;

    static const std::int16_t WARPCD = 48;
    std::int16_t cooldown;
};
} // namespace jrc
