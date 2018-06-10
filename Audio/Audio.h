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
#include "../Error.h"
#include "../Template/EnumMap.h"
#include "nlnx/node.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace jrc
{
class Sound
{
public:
    // Preloaded sounds.
    enum Name {
        // UI
        BUTTONCLICK,
        BUTTONOVER,

        // Login
        SELECTCHAR,
        GAMESTART,

        // Game
        JUMP,
        DROP,
        PICKUP,
        PORTAL,
        LEVELUP,
        LENGTH
    };

    Sound(Name name);
    Sound(nl::node src);
    Sound();

    void play() const;

    static Error init();
    static void close();
    static bool set_sfx_volume(std::uint8_t volume);

private:
    std::size_t id;

    static void play(std::size_t id);

    static std::size_t add_sound(nl::node src);
    static void add_sound(Sound::Name name, nl::node src);

    static std::unordered_map<std::size_t, std::uint64_t> samples;
    static EnumMap<Name, std::size_t> sound_ids;
};

class Music
{
public:
    Music(std::string&& path) noexcept;

    void play() const;

    static Error init();
    static bool set_bgm_volume(std::uint8_t volume);

private:
    std::string path;
};
} // namespace jrc
