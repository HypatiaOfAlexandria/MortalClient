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
#include <cstdint>

namespace jrc
{
namespace Constants
{
//! Timestep, e.g. the granularity in which the game advances.
constexpr std::uint16_t TIMESTEP = 8;

//! Initial window and screen width.
constexpr std::int16_t VIEW_WIDTH = 800;

//! Initial window and screen height.
constexpr std::int16_t VIEW_HEIGHT = 600;

//! In-game window and screen width.
constexpr std::int16_t GAME_VIEW_WIDTH = 1024;

//! In-game window and screen height.
constexpr std::int16_t GAME_VIEW_HEIGHT = 768;

//! View y-offset.
constexpr std::int16_t VIEW_Y_OFFSET = 10;
} // namespace Constants
} // namespace jrc
