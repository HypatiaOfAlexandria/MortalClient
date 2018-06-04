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
#include "../Error.h"
#include "../Journey.h"
#include "../Template/Singleton.h"
#include "Cryptography.h"
#include "PacketSwitch.h"
#ifdef JOURNEY_USE_ASIO
#    include "SocketAsio.h"
#else
#    include "SocketWinsock.h"
#endif

namespace jrc
{
class Session : public Singleton<Session>
{
public:
    Session();
    ~Session() override;

    // Connect using host and port from the configuration file.
    Error init();
    // Send a packet to the server.
    void write(std::int8_t* bytes, std::size_t length);
    // Check for incoming packets and handle them.
    void read();
    // Closes the current connection and opens a new one.
    void reconnect(const char* address, const char* port);
    // Check if the connection is alive.
    bool is_connected() const;

private:
    bool init(const char* host, const char* port);
    void process(const std::int8_t* bytes, std::size_t available);

    Cryptography cryptography;
    PacketSwitch packetswitch;

    std::int8_t buffer[MAX_PACKET_LENGTH];
    std::size_t length;
    std::size_t pos;
    bool connected;

#ifdef JOURNEY_USE_ASIO
    SocketAsio socket;
#else
    SocketWinsock socket;
#endif
};
} // namespace jrc
