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
#include "../../Graphics/Sprite.h"
#include "../../Graphics/Texture.h"
#include "../UIElement.h"

namespace jrc
{
class UILoginWait : public UIElement
{
public:
    static constexpr const Type TYPE = LOGIN_WAIT;
    static constexpr const bool FOCUSED = true;
    static constexpr const bool TOGGLED = false;

    UILoginWait();

    void draw(float alpha) const override;
    void update() override;

private:
    Texture background;
    Sprite circle;
};
} // namespace jrc
