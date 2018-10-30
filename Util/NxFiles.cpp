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
#include "NxFiles.h"

#include "../Console.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

#include <cstdio>
#include <fstream>
#ifndef _WIN32
#    include <unistd.h>
#else
#    include <io.h>
#endif

namespace jrc
{
Error NxFiles::init()
{
    for (auto filename : NxFiles::filenames) {
        if (!std::ifstream{filename}.good()) {
            return {Error::MISSING_FILE, filename};
        }
    }

    try {
        nl::nx::load_all();
    } catch (const std::exception& ex) {
        static const std::string message = ex.what();

        return {Error::NLNX, message.c_str()};
    }

    constexpr const std::string_view POSTCHAOS_BITMAP
        = "Login.img/WorldSelect/BtChannel/layer:bg";
    auto postChaosBitmapType
        = nl::nx::ui.resolve(POSTCHAOS_BITMAP).data_type();

    if (postChaosBitmapType != nl::node::type::bitmap) {
        return Error::WRONG_UI_FILE;
    }

    return Error::NONE;
}
} // namespace jrc
