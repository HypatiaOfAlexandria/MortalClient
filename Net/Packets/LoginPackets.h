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
#pragma once
#include "../OutPacket.h"

namespace jrc
{
// Accept the Terms of Service.
// Opcode: ACCEPT_TOS(7)
class TOSPacket : public OutPacket
{
public:
    TOSPacket() : OutPacket(ACCEPT_TOS)
    {
        write_byte(1);
    }
};

// Request to be logged-in to an account.
// Opcode: LOGIN(1) [LOGIN_PASSWORD(1) for OdinMS-based sources]
class LoginPacket : public OutPacket
{
public:
    LoginPacket(const std::string& acc, const std::string& pass)
        : OutPacket(LOGIN)
    {
        write_string(acc);
        write_string(pass);
    }
};

// Requests the list of worlds and channels.
// Opcode: SERVERLIST_REQUEST(11)
class ServerRequestPacket : public OutPacket
{
public:
    ServerRequestPacket() : OutPacket(SERVERLIST_REQUEST)
    {
    }
};

// Requests the list of characters on a world.
// Opcode: CHARLIST_REQUEST(5)
class CharlistRequestPacket : public OutPacket
{
public:
    CharlistRequestPacket(std::uint8_t world, std::uint8_t channel)
        : OutPacket(CHARLIST_REQUEST)
    {
        write_byte(0);
        write_byte(world);
        write_byte(channel);
    }
};

// Requests being logged-in to a channel server with the specified character.
// Opcode: PLAYER_LOGIN(20) [PLAYER_LOGGEDIN(20) for OdinMS-based sources]
class PlayerLoginPacket : public OutPacket
{
public:
    PlayerLoginPacket(std::int32_t cid) : OutPacket(PLAYER_LOGIN)
    {
        write_int(cid);
    }
};
} // namespace jrc
