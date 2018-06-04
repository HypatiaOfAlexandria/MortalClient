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
#include "../Character/Inventory/Weapon.h"
#include "../Gameplay/Combat/SpecialMove.h"
#include "../Template/EnumMap.h"

#include <string>

namespace jrc
{
namespace Messages
{
enum Type {
    NONE,
    // Cannot use a skill
    SKILL_WEAPONTYPE,
    SKILL_HPCOST,
    SKILL_MPCOST,
    SKILL_NOARROWS,
    SKILL_NOBULLETS,
    SKILL_NOSTARS,
    SKILL_COOLDOWN,

    // Scrolling result
    SCROLL_SUCCESS,
    SCROLL_FAILURE,
    SCROLL_DESTROYED,
    LENGTH
};

extern const EnumMap<Type, const char*> messages;
}; // namespace Messages

class InChatMessage
{
public:
    InChatMessage(Messages::Type type);

    void drop() const;

private:
    Messages::Type type;
};

class ForbidSkillMessage : public InChatMessage
{
public:
    ForbidSkillMessage(SpecialMove::ForbidReason reason, Weapon::Type weapon);

private:
    Messages::Type message_by_reason(SpecialMove::ForbidReason reason,
                                     Weapon::Type weapon);
    Messages::Type message_by_weapon(Weapon::Type weapon);
};
} // namespace jrc
