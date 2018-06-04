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
#include "../../Graphics/Texture.h"
#include "BodyDrawInfo.h"

namespace jrc
{
class Hair
{
public:
    enum Layer {
        NONE,
        DEFAULT,
        BELOWBODY,
        OVERHEAD,
        SHADE,
        BACK,
        BELOWCAP,
        NUM_LAYERS
    };

    Hair(std::int32_t hairid, const BodyDrawinfo& drawinfo);

    void draw(Stance::Id stance,
              Layer layer,
              std::uint8_t frame,
              const DrawArgument& args) const;

    const std::string& get_name() const;
    const std::string& getcolor() const;

private:
    std::unordered_map<std::uint8_t, Texture> stances[Stance::LENGTH]
                                                     [Layer::NUM_LAYERS];
    std::string name;
    std::string color;

    static const std::unordered_map<std::string, Layer> layers_by_name;
};
} // namespace jrc
