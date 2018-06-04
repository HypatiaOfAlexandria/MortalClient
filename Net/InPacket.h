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
#include "../Template/Point.h"
#include "PacketError.h"

#include <cstdint>
#include <optional>

namespace jrc
{
/// A packet received from the server.
/// Contains reading functions.
class InPacket
{
public:
    /// Construct a packet from an array of bytes.
    InPacket(const std::int8_t* bytes, std::size_t length);

    /// Check if there are more bytes available.
    bool available() const;
    /// Return the remaining length in bytes.
    std::size_t length() const;
    /// Skip a number of bytes (by increasing the offset).
    void skip(std::size_t count);

    /// Read a byte and check if it is 1.
    bool read_bool();
    /// Read a byte.
    std::int8_t read_byte();
    /// Read a short.
    std::int16_t read_short();
    /// Read an int.
    std::int32_t read_int();
    /// Read a long.
    std::int64_t read_long();

    /// Read a point.
    Point<std::int16_t> read_point();

    /// Read a string.
    std::string read_string();
    /// Read a fixed-length string.
    std::string read_padded_string(std::uint16_t length);

    /// Inspect a byte and check if it is 1. Does not advance the buffer
    /// position.
    bool inspect_bool();
    /// Inspect a byte. Does not advance the buffer position.
    std::int8_t inspect_byte();
    /// Inspect a short. Does not advance the buffer position.
    std::int16_t inspect_short();
    /// Inspect an int. Does not advance the buffer position.
    std::int32_t inspect_int();
    /// Inspect a long. Does not advance the buffer position.
    std::int64_t inspect_long();

private:
    template<typename T>
    /// Read a number and advance the buffer position.
    T read()
    {
        constexpr std::size_t count = sizeof(T) / sizeof(std::int8_t);
        T all = 0;
        for (std::size_t i = 0; i < count; ++i) {
            T val = static_cast<std::uint8_t>(bytes[pos]);
            all += val << (8 * i);

            skip(1);
        }

        return all;
    }

    template<typename T>
    /// Read without advancing the buffer position.
    T inspect()
    {
        std::size_t before = pos;
        T value = read<T>();
        pos = before;
        return value;
    }

    const std::int8_t* bytes;
    std::size_t top;
    std::size_t pos;
};
} // namespace jrc
