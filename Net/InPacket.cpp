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
#include "InPacket.h"

namespace jrc
{
InPacket::InPacket(const std::int8_t* recv, std::size_t length) noexcept
    : bytes(recv), top(length), pos(0)
{
}

bool InPacket::available() const
{
    return length() > 0;
}

std::size_t InPacket::length() const
{
    return top - pos;
}

void InPacket::skip(std::size_t count) noexcept(false)
{
    if (count > length()) {
        throw PacketError("Stack underflow at " + std::to_string(pos));
    }

    pos += count;
}

bool InPacket::read_bool()
{
    return read_byte() == 1;
}

std::int8_t InPacket::read_byte()
{
    return read<std::int8_t>();
}

std::int16_t InPacket::read_short()
{
    return read<std::int16_t>();
}

std::int32_t InPacket::read_int()
{
    return read<std::int32_t>();
}

std::int64_t InPacket::read_long()
{
    return read<std::int64_t>();
}

Point<std::int16_t> InPacket::read_point()
{
    auto x = read<std::int16_t>();
    auto y = read<std::int16_t>();
    return {x, y};
}

std::string InPacket::read_string()
{
    auto length = read<std::uint16_t>();
    return read_padded_string(length);
}

std::string InPacket::read_string_raw()
{
    auto length = read<std::uint16_t>();

    std::string ret;
    ret.reserve(static_cast<std::string::size_type>(length));

    for (std::uint16_t i = 0; i < length; ++i) {
        ret.push_back(static_cast<char>(read_byte()));
    }

    return ret;
}

std::string InPacket::read_padded_string(std::uint16_t count)
{
    std::string ret;
    ret.reserve(static_cast<std::string::size_type>(count));

    for (std::uint16_t i = 0; i < count; ++i) {
        char letter = static_cast<char>(read_byte());
        if (letter != '\0') {
            ret.push_back(letter);
        }
    }

    return ret;
}

bool InPacket::inspect_bool()
{
    return inspect_byte() == 1;
}

std::int8_t InPacket::inspect_byte()
{
    return inspect<std::int8_t>();
}

std::int16_t InPacket::inspect_short()
{
    return inspect<std::int16_t>();
}

std::int32_t InPacket::inspect_int()
{
    return inspect<std::int32_t>();
}

std::int64_t InPacket::inspect_long()
{
    return inspect<std::int64_t>();
}
} // namespace jrc
