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
#include <array>
#include <type_traits>

namespace jrc
{
template<typename E, E LENGTH = E::LENGTH>
//! Makes contiguous `enum`s iterable.
class Enumeration
{
public:
    using underlying_t = typename std::array<E, LENGTH>;

    template<std::size_t... VS>
    constexpr Enumeration(std::index_sequence<VS...>) noexcept
        : values{{static_cast<E>(VS)...}}
    {
    }

    constexpr Enumeration() noexcept
        : Enumeration(std::make_index_sequence<LENGTH>{})
    {
        static_assert(std::is_enum<E>::value,
                      "Template parameter E for Enumeration must be an enum");
    }

    auto begin() const noexcept
    {
        return values.begin();
    }

    auto end() const noexcept
    {
        return values.end();
    }

    auto cbegin() const noexcept
    {
        return values.cbegin();
    }

    auto cend() const noexcept
    {
        return values.cend();
    }

private:
    underlying_t values;
};
} // namespace jrc
