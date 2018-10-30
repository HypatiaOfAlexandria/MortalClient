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
#include <utility>

namespace jrc
{
template<typename T>
class BoolPair
{
public:
    template<typename... Args>
    BoolPair(Args&&... args_f, Args&&... args_s) noexcept(
        noexcept(T(std::forward<Args>(args_f)...)))
        : first(std::forward<Args>(args_f)...),
          second(std::forward<Args>(args_s)...)
    {
    }

    BoolPair(T f, T s) noexcept(noexcept(T(f))) : first(f), second(s)
    {
    }

    BoolPair() = default;

    template<typename... Args>
    void set(bool b,
             Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
    {
        if (b) {
            first = T(std::forward<Args>(args)...);
        } else {
            second = T(std::forward<Args>(args)...);
        }
    }

    T& operator[](bool b) noexcept
    {
        return b ? first : second;
    }

    const T& operator[](bool b) const noexcept
    {
        return b ? first : second;
    }

private:
    T first;
    T second;
};
} // namespace jrc
