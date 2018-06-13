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
#include "Audio.h"

#include "../Configuration.h"

#define WIN32_LEAN_AND_MEAN
#include "bass/bass.h"
#include "nlnx/audio.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
constexpr const char* Error::messages[];

Sound::Sound(Name name) : id(sound_ids[name])
{
}

Sound::Sound(nl::node src) : id(add_sound(src))
{
}

Sound::Sound() : id(0)
{
}

void Sound::play() const
{
    if (id > 0) {
        play(id);
    }
}

Error Sound::init()
{
    if (!BASS_Init(1, 44100, 0, nullptr, nullptr)) {
        return Error::AUDIO;
    }

    nl::node uisrc = nl::nx::sound["UI.img"];

    add_sound(Sound::BUTTONCLICK, uisrc["BtMouseClick"]);
    add_sound(Sound::BUTTONOVER, uisrc["BtMouseOver"]);
    add_sound(Sound::SELECTCHAR, uisrc["CharSelect"]);

    nl::node gamesrc = nl::nx::sound["Game.img"];

    add_sound(Sound::GAMESTART, gamesrc["GameIn"]);
    add_sound(Sound::JUMP, gamesrc["Jump"]);
    add_sound(Sound::DROP, gamesrc["DropItem"]);
    add_sound(Sound::PICKUP, gamesrc["PickUpItem"]);
    add_sound(Sound::PORTAL, gamesrc["Portal"]);
    add_sound(Sound::LEVELUP, gamesrc["LevelUp"]);

    std::uint8_t volume = Setting<SFXVolume>::get().load();

    if (!set_sfx_volume(volume)) {
        return Error::AUDIO;
    }

    return Error::NONE;
}

void Sound::close()
{
    BASS_Free();
}

bool Sound::set_sfx_volume(std::uint8_t vol)
{
    return BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol * 100u) == TRUE;
}

void Sound::play(std::size_t id)
{
    if (!samples.count(id)) {
        return;
    }

    HCHANNEL channel =
        BASS_SampleGetChannel(static_cast<HSAMPLE>(samples.at(id)), false);
    BASS_ChannelPlay(channel, true);
}

std::size_t Sound::add_sound(nl::node src)
{
    nl::audio ad = src;

    const void* data = ad.data();

    if (data) {
        std::size_t id = ad.id();

        samples[id] = BASS_SampleLoad(true,
                                      data,
                                      82,
                                      static_cast<DWORD>(ad.length()),
                                      4,
                                      BASS_SAMPLE_OVER_POS);

        return id;
    } else {
        return 0;
    }
}

void Sound::add_sound(Name name, nl::node src)
{
    std::size_t id = add_sound(src);

    if (id) {
        sound_ids[name] = id;
    }
}

std::unordered_map<std::size_t, std::uint64_t> Sound::samples;
EnumMap<Sound::Name, std::size_t> Sound::sound_ids;

Music::Music(std::string&& p) noexcept : path(std::move(p))
{
}

void Music::play() const
{
    static HSTREAM stream = 0;
    static std::string bgm_path;

    if (path == bgm_path) {
        return;
    }

    nl::audio ad = nl::nx::sound.resolve(path);
    const void* data = ad.data();

    if (data) {
        if (stream) {
            BASS_ChannelStop(stream);
            BASS_StreamFree(stream);
        }

        stream = BASS_StreamCreateFile(
            true, data, 82, ad.length(), BASS_SAMPLE_FLOAT | BASS_SAMPLE_LOOP);
        BASS_ChannelPlay(stream, true);

        bgm_path = path;
    }
}

Error Music::init()
{
    std::uint8_t volume = Setting<BGMVolume>::get().load();

    if (!set_bgm_volume(volume)) {
        return Error::AUDIO;
    }

    return Error::NONE;
}

bool Music::set_bgm_volume(std::uint8_t vol)
{
    return BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol * 100u) == TRUE;
}
} // namespace jrc
