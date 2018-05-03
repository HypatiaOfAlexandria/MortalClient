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
#include "Configuration.h"

#include <fstream>

namespace jrc
{
Configuration::Configuration()
{
    settings.emplace<ServerIP>();
    settings.emplace<Fullscreen>();
    settings.emplace<VSync>();
    settings.emplace<FontPathNormal>();
    settings.emplace<FontPathBold>();
    settings.emplace<BGMVolume>();
    settings.emplace<SFXVolume>();
    settings.emplace<SaveLogin>();
    settings.emplace<DefaultAccount>();
    settings.emplace<DefaultWorld>();
    settings.emplace<DefaultChannel>();
    settings.emplace<DefaultCharacter>();
    settings.emplace<PosSTATS>();
    settings.emplace<PosEQINV>();
    settings.emplace<PosINV>();
    settings.emplace<PosSKILL>();

    load();
}

Configuration::~Configuration()
{
    save();
}

void Configuration::load()
{
    std::unordered_map<std::string, std::string> rawsettings;

    std::ifstream file(FILENAME);
    if (file.is_open()) {
        // Go through the file line for line.
        std::string line;
        while (getline(file, line)) {
            // If the setting is not empty, load the value.
            size_t split = line.find('=');
            if (split != std::string::npos && split + 2 < line.size()) {
                rawsettings.emplace(line.substr(0, split - 1),
                                    line.substr(split + 2));
            }
        }
    }

    // Replace default values with loaded values.
    for (auto& setting : settings) {
        auto rsiter = rawsettings.find(setting.second->name);
        if (rsiter != rawsettings.end()) {
            setting.second->value = rsiter->second;
        }
    }
}

void Configuration::save() const
{
    // Open the settings file.
    std::ofstream config(FILENAME);
    if (config.is_open()) {
        // Save settings line by line.
        for (auto& setting : settings) {
            config << setting.second->to_string() << '\n';
        }
    }
}

void Configuration::BoolEntry::save(bool b)
{
    value = b ? "true" : "false";
}

bool Configuration::BoolEntry::load() const
{
    return value == "true";
}

void Configuration::StringEntry::save(std::string str)
{
    value = str;
}

std::string Configuration::StringEntry::load() const
{
    return value;
}

void Configuration::PointEntry::save(Point<int16_t> vec)
{
    value = vec.to_string();
}

Point<int16_t> Configuration::PointEntry::load() const
{
    std::string xstr = value.substr(1, value.find(",") - 1);
    std::string ystr = value.substr(value.find(",") + 1,
                                    value.find(")") - value.find(",") - 1);

    auto x = string_conversion::or_zero<int16_t>(xstr);
    auto y = string_conversion::or_zero<int16_t>(ystr);
    return {x, y};
}
} // namespace jrc
