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
#include "Body.h"

#include "../../Console.h"
#include "../../Util/Misc.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
Body::Body(int32_t skin, const BodyDrawinfo& drawinfo)
{
    std::string strid = string_format::extend_id(skin, 2);
    nl::node bodynode = nl::nx::character["000020" + strid + ".img"];
    nl::node headnode = nl::nx::character["000120" + strid + ".img"];

    for (auto iter : Stance::names) {
        Stance::Id stance = iter.first;
        const std::string& stancename = iter.second;

        nl::node stancenode = bodynode[stancename];
        if (!stancenode)
            continue;

        for (uint8_t frame = 0; nl::node framenode = stancenode[frame];
             ++frame) {
            for (nl::node partnode : framenode) {
                std::string part = partnode.name();
                if (part != "delay" && part != "face") {
                    std::string z = partnode["z"];
                    Layer layer = layer_by_name(z);
                    if (layer == Layer::NONE)
                        continue;

                    Point<int16_t> shift;
                    switch (layer) {
                    case HAND_BELOW_WEAPON:
                        shift = drawinfo.get_hand_position(stance, frame);
                        shift -= partnode["map"]["handMove"];
                        break;
                    default:
                        shift = drawinfo.get_body_position(stance, frame);
                        shift -= partnode["map"]["navel"];
                        break;
                    }

                    stances[stance][layer]
                        .emplace(frame, partnode)
                        .first->second.shift(shift);
                }
            }

            if (nl::node headsfnode = headnode[stancename][frame]["head"]) {
                Point<int16_t> shift =
                    drawinfo.get_head_position(stance, frame);

                stances[stance][Layer::HEAD]
                    .emplace(frame, headsfnode)
                    .first->second.shift(shift);
            }
        }
    }

    constexpr size_t NUM_SKINTYPES = 12;
    constexpr char const* skintypes[NUM_SKINTYPES] = {"Light",
                                                      "Tan",
                                                      "Dark",
                                                      "Pale",
                                                      "Blue",
                                                      "Green",
                                                      "",
                                                      "",
                                                      "",
                                                      "Grey",
                                                      "Pink",
                                                      "Red"};
    size_t index = skin;
    name = (index < NUM_SKINTYPES) ? skintypes[index] : "";
}

void Body::draw(Stance::Id stance,
                Layer layer,
                uint8_t frame,
                const DrawArgument& args) const
{
    auto frameit = stances[stance][layer].find(frame);
    if (frameit == stances[stance][layer].end())
        return;

    frameit->second.draw(args);
}

const std::string& Body::get_name() const
{
    return name;
}

Body::Layer Body::layer_by_name(const std::string& name)
{
    auto layer_iter = layers_by_name.find(name);
    if (layer_iter == layers_by_name.end()) {
        Console::get().print("Warning: Unhandled body layer (" + name + ")");
        return Layer::NONE;
    }
    return layer_iter->second;
}

const std::unordered_map<std::string, Body::Layer> Body::layers_by_name = {
    {"body", Body::BODY},
    {"backBody", Body::BODY},
    {"arm", Body::ARM},
    {"armBelowHead", Body::ARM_BELOW_HEAD},
    {"armBelowHeadOverMailChest", Body::ARM_BELOW_HEAD_OVER_MAIL},
    {"armOverHair", Body::ARM_OVER_HAIR},
    {"armOverHairBelowWeapon", Body::ARM_OVER_HAIR_BELOW_WEAPON},
    {"handBelowWeapon", Body::HAND_BELOW_WEAPON},
    {"handOverHair", Body::HAND_OVER_HAIR},
    {"handOverWeapon", Body::HAND_OVER_WEAPON},
    {"head", Body::HEAD}};
} // namespace jrc
