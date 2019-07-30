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
// along with this program.  If not, see <https:///www.gnu.org/licenses/>.  //
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../Gameplay/Combat/DamageNumber.h"
#include "../Gameplay/MapleMap/MapObject.h"
#include "../Graphics/EffectLayer.h"
#include "../IO/Components/ChatBalloon.h"
#include "../Template/EnumMap.h"
#include "../Template/Rectangle.h"
#include "../Util/TimedBool.h"
#include "CharEffect.h"
#include "Inventory/Weapon.h"
#include "Look/Afterimage.h"
#include "Look/CharLook.h"
#include "Look/PetLook.h"
#include "tinyutf8.hpp"

#include <vector>

namespace jrc
{
//! Base for characters, e.g. the player and other clients on the same map.
class Char : public MapObject
{
public:
    //! Player states which determine animation and state.
    //! Values are used in movement packets (add 1 if facing left).
    enum State : std::int8_t {
        WALK = 2,
        STAND = 4,
        FALL = 6,
        ALERT = 8,
        PRONE = 10,
        SWIM = 12,
        LADDER = 14,
        ROPE = 16,
        DIED = 18,
        SIT = 20
    };

    static State by_value(std::int8_t value) noexcept
    {
        return static_cast<State>(value);
    }

    //! Draw look, nametag, effects and chat bubble.
    void draw(double viewx, double viewy, float alpha) const override;
    //! Update look and movements.
    std::int8_t update(const Physics& physics) override;
    //! Return the current map layer, or 7 if on a ladder or rope.
    std::int8_t get_layer() const override;

    //! Check wether the character is invincible.
    virtual bool is_invincible() const;
    //! Return the character's level.
    virtual std::uint16_t get_level() const = 0;
    //! Return the character's level.
    virtual std::int32_t get_skill_level(std::int32_t skillid) const = 0;
    //! Return the character's base attacking speed.
    virtual std::int8_t get_integer_attack_speed() const = 0;

    //! Return the attack speed as a multiplier.
    float get_real_attack_speed() const;
    //! Return the delay until applying an attack.
    std::uint16_t get_attack_delay(std::size_t no) const;

    //! Set if the character sprite is mirrored (true = facing left)
    virtual void set_direction(bool flipped);
    //! Change the character's state.
    virtual void set_state(State newstate);
    //! Change the character's stance to an attack action.
    void attack(const std::string& action);
    //! Change the character's stance to an attack stance.
    void attack(Stance::Id stance);
    //! Change the character's stance to it's regular attack.
    void attack(bool degenerate);
    //! Set the afterimage for an attack.
    void set_afterimage(std::int32_t skill_id);
    //! Return the current afterimage.
    const Afterimage& get_afterimage() const;

    //! Display an animation as an effect with the character.
    void show_attack_effect(Animation to_show, std::int8_t z);
    //! Display an animation as an effect ontop of the character.
    void show_effect_id(CharEffect::Id to_show);
    //! Display the iron body skill animation.
    void show_iron_body();
    //! Display damage over the characters head.
    void show_damage(std::int32_t damage);
    //! Display a chat bubble with the specified line in it.
    void speak(utf8_string&& line);
    //! Change a part of the character's look.
    void change_look(Maplestat::Id stat, std::int32_t id);
    //! Change the character's state by id.
    void set_state(std::uint8_t state_byte);
    //! Change the character's face expression by id.
    void set_expression(std::int32_t exp_id);

    //! Add a pet with the specified stats.
    void add_pet(std::uint8_t index,
                 std::int32_t iid,
                 utf8_string&& name,
                 std::int32_t uniqueid,
                 Point<std::int16_t> pos,
                 std::uint8_t stance,
                 std::int32_t fhid);
    //! Remove a pet with the specified index and reason.
    void remove_pet(std::uint8_t index, bool hunger);

    //! Return if the character is facing left.
    bool get_flip() const;
    //! Return the name of this character.
    const utf8_string& get_name() const;

    //! Return if the char is in the Char::SIT state.
    bool is_sitting() const;
    //! Return if the char is in the Char::LADDER or Char::ROPE state.
    bool is_climbing() const;
    //! Return wether the character sprite uses stances for two-handed weapons.
    bool is_two_handed() const;
    //! Return the type of the currently equipped weapon.
    Weapon::Type get_weapon_type() const;

    //! Obtain a reference to this character's look.
    CharLook& get_look();
    //! Obtain a const reference to this character's look.
    const CharLook& get_look() const;
    //! Return a reference to this characters's physics.
    PhysicsObject& get_phobj();

    //! Initialize character effects.
    static void init();

protected:
    Char(std::int32_t oid, const CharLook& look, utf8_string&& name) noexcept;

    //! Update the character's animation with the given speed.
    bool update(const Physics& physics, float speed);
    //! Get a speed modifier for the current stance.
    float get_stance_speed() const;

    CharLook look;
    PetLook pets[3];

    State state;
    bool attacking;
    bool flip;

private:
    Text name_label;
    ChatBalloon chat_balloon;
    EffectLayer effects;
    Afterimage afterimage;
    TimedBool invincible;
    TimedBool iron_body;
    std::vector<DamageNumber> damage_numbers;

    static EnumMap<CharEffect::Id, Animation> char_effects;
};
} // namespace jrc
