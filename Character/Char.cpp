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
#include "Char.h"

#include "../Constants.h"
#include "../Data/WeaponData.h"
#include "../Util/Misc.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

#include <math.h>

namespace jrc
{
Char::Char(int32_t o, const CharLook& lk, const std::string& name)
    : MapObject(o),
      look(lk),
      namelabel({Text::A13M, Text::CENTER, Text::WHITE, Text::NAMETAG, name})
{
}

void Char::draw(double viewx, double viewy, float alpha) const
{
    Point<int16_t> absp = phobj.get_absolute(viewx, viewy, alpha);

    effects.drawbelow(absp, alpha);

    Color color;
    if (invincible) {
        float phi = invincible.alpha() * 30;
        float rgb = 0.9f - 0.5f * std::abs(sinf(phi));
        color = {rgb, rgb, rgb, 1.0f};
    } else {
        color = Color::WHITE;
    }
    look.draw({absp, color}, alpha);

    afterimage.draw(look.get_frame(), {absp, flip}, alpha);

    if (ironbody) {
        float ibalpha = ironbody.alpha();
        float scale = 1.0f + ibalpha;
        float opacity = 1.0f - ibalpha;
        look.draw({absp, scale, scale, opacity}, alpha);
    }

    for (auto& pet : pets) {
        if (pet.get_itemid()) {
            pet.draw(viewx, viewy, alpha);
        }
    }

    namelabel.draw(absp);
    chatballoon.draw(absp - Point<int16_t>(0, 85));

    effects.drawabove(absp, alpha);

    for (auto& number : damagenumbers) {
        number.draw(viewx, viewy, alpha);
    }
}

bool Char::update(const Physics& physics, float speed)
{
    damagenumbers.remove_if(
        [](DamageNumber& number) { return number.update(); });
    effects.update();
    chatballoon.update();
    invincible.update();
    ironbody.update();

    for (auto& pet : pets) {
        if (pet.get_itemid()) {
            switch (state) {
            case LADDER:
            case ROPE:
                pet.set_stance(PetLook::HANG);
                break;
            case SWIM:
                pet.set_stance(PetLook::FLY);
                break;
            default:
                if (pet.get_stance() == PetLook::HANG ||
                    pet.get_stance() == PetLook::FLY)
                    pet.set_stance(PetLook::STAND);
            }
            pet.update(physics, get_position());
        }
    }

    uint16_t stancespeed = 0;
    if (speed >= 1.0f / Constants::TIMESTEP) {
        stancespeed = static_cast<uint16_t>(Constants::TIMESTEP * speed);
    }
    afterimage.update(look.get_frame(), stancespeed);
    return look.update(stancespeed);
}

float Char::get_stancespeed() const
{
    if (attacking)
        return get_real_attackspeed();

    switch (state) {
    case WALK:
        return static_cast<float>(std::abs(phobj.hspeed));
    case LADDER:
    case ROPE:
        return static_cast<float>(std::abs(phobj.vspeed));
    default:
        return 1.0f;
    }
}

float Char::get_real_attackspeed() const
{
    int8_t speed = get_integer_attackspeed();
    return 1.7f - static_cast<float>(speed) / 10;
}

uint16_t Char::get_attackdelay(size_t no) const
{
    uint8_t first_frame = afterimage.get_first_frame();
    uint16_t delay = look.get_attackdelay(no, first_frame);
    float fspeed = get_real_attackspeed();
    return static_cast<uint16_t>(delay / fspeed);
}

int8_t Char::update(const Physics& physics)
{
    update(physics, 1.0f);
    return get_layer();
}

int8_t Char::get_layer() const
{
    return is_climbing() ? 7 : phobj.fhlayer;
}

void Char::show_attack_effect(Animation toshow, int8_t z)
{
    float attackspeed = get_real_attackspeed();
    effects.add(toshow, {flip}, z, attackspeed);
}

void Char::show_effect_id(CharEffect::Id toshow)
{
    effects.add(chareffects[toshow]);
}

void Char::show_iron_body()
{
    ironbody.set_for(500);
}

void Char::show_damage(int32_t damage)
{
    int16_t start_y = phobj.get_y() - 60;
    int16_t x = phobj.get_x() - 10;
    damagenumbers.emplace_back(DamageNumber::TOPLAYER, damage, start_y, x);

    look.set_alerted(5000);
    invincible.set_for(2000);
}

void Char::speak(const std::string& line)
{
    chatballoon.change_text(line);
}

void Char::change_look(Maplestat::Id stat, int32_t id)
{
    switch (stat) {
    case Maplestat::SKIN:
        look.set_body(id);
        break;
    case Maplestat::FACE:
        look.set_face(id);
        break;
    case Maplestat::HAIR:
        look.set_hair(id);
        break;
    }
}

void Char::set_state(uint8_t statebyte)
{
    if (statebyte % 2 == 1) {
        set_direction(false);
        statebyte -= 1;
    } else {
        set_direction(true);
    }

    Char::State newstate = by_value(statebyte);
    set_state(newstate);
}

void Char::set_expression(int32_t expid)
{
    Expression::Id expression = Expression::byaction(expid);
    look.set_expression(expression);
}

void Char::attack(const std::string& action)
{
    look.set_action(action);

    attacking = true;
    look.set_alerted(5000);
}

void Char::attack(Stance::Id stance)
{
    look.attack(stance);

    attacking = true;
    look.set_alerted(5000);
}

void Char::attack(bool degenerate)
{
    look.attack(degenerate);

    attacking = true;
    look.set_alerted(5000);
}

void Char::set_afterimage(int32_t skill_id)
{
    int32_t weapon_id = look.get_equips().get_weapon();
    if (weapon_id <= 0)
        return;

    const WeaponData& weapon = WeaponData::get(weapon_id);

    std::string stance_name = Stance::names[look.get_stance()];
    int16_t weapon_level =
        weapon.get_equipdata().get_reqstat(Maplestat::LEVEL);
    const std::string& ai_name = weapon.get_afterimage();
    afterimage = {skill_id, ai_name, stance_name, weapon_level};
}

const Afterimage& Char::get_afterimage() const
{
    return afterimage;
}

void Char::set_direction(bool f)
{
    flip = f;
    look.set_direction(f);
}

void Char::set_state(State st)
{
    state = st;

    Stance::Id stance = Stance::by_state(state);
    look.set_stance(stance);
}

void Char::add_pet(uint8_t index,
                   int32_t iid,
                   const std::string& name,
                   int32_t uniqueid,
                   Point<int16_t> pos,
                   uint8_t stance,
                   int32_t fhid)
{
    if (index > 2)
        return;

    pets[index] = PetLook(iid, name, uniqueid, pos, stance, fhid);
}

void Char::remove_pet(uint8_t index, bool hunger)
{
    if (index > 2)
        return;

    pets[index] = PetLook();
    if (hunger) {
    }
}

bool Char::is_invincible() const
{
    return invincible == true;
}

bool Char::is_sitting() const
{
    return state == SIT;
}

bool Char::is_climbing() const
{
    return state == LADDER || state == ROPE;
}

bool Char::is_twohanded() const
{
    return look.get_equips().is_twohanded();
}

Weapon::Type Char::get_weapontype() const
{
    int32_t weapon_id = look.get_equips().get_weapon();
    if (weapon_id <= 0)
        return Weapon::NONE;

    return WeaponData::get(weapon_id).get_type();
}

bool Char::getflip() const
{
    return flip;
}

std::string Char::get_name() const
{
    return namelabel.get_text();
}

CharLook& Char::get_look()
{
    return look;
}

const CharLook& Char::get_look() const
{
    return look;
}

PhysicsObject& Char::get_phobj()
{
    return phobj;
}

void Char::init()
{
    CharLook::init();

    nl::node src = nl::nx::effect["BasicEff.img"];
    for (auto iter : CharEffect::PATHS) {
        chareffects.emplace(iter.first, src.resolve(iter.second));
    }
}

EnumMap<CharEffect::Id, Animation> Char::chareffects;
} // namespace jrc
