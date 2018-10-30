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
#include "Stage.h"

#include "../Audio/Audio.h"
#include "../Character/SkillId.h"
#include "../IO/Messages.h"
#include "../Net/Packets/AttackAndSkillPackets.h"
#include "../Net/Packets/GameplayPackets.h"
#include "../Util/Misc.h"
#include "nlnx/nx.hpp"

#include <iostream>

namespace jrc
{
Stage::Stage() : combat(player, chars, mobs)
{
    state = INACTIVE;
}

void Stage::init()
{
    drops.init();
}

void Stage::load(std::int32_t map_id, std::int8_t portal_id)
{
    switch (state) {
    case INACTIVE:
        load_map(map_id);
        respawn(portal_id);
        break;
    case TRANSITION:
        respawn(portal_id);
        break;
    case ACTIVE:
        break;
    }

    state = ACTIVE;
}

void Stage::loadplayer(const CharEntry& entry)
{
    player = entry;
    playable = player;
}

void Stage::clear()
{
    state = INACTIVE;

    chars.clear();
    npcs.clear();
    mobs.clear();
    drops.clear();
    reactors.clear();
}

void Stage::load_map(std::int32_t map_id)
{
    std::string str_id = string_format::extend_id(map_id, 9);
    str_id += ".img";

    nl::node src
        = nl::nx::map["Map"]["Map" + std::to_string(map_id / 100'000'000)]
                     [str_id];

    tiles_objs = MapTilesObjs(src);
    backgrounds = MapBackgrounds(src["back"]);
    physics = Physics(src["foothold"]);
    map_info = {
        src, physics.get_fht().get_walls(), physics.get_fht().get_borders()};
    portals = MapPortals(src["portal"], map_id);
}

void Stage::respawn(std::int8_t portal_id)
{
    std::string bgm_path = map_info.get_bgm();
    if (auto mus_err = Music::play(bgm_path); mus_err) {
        Console::get().print("Error playing music " + bgm_path);
    }

    Point<std::int16_t> spawn_point
        = portals.get_portal_by_id(static_cast<std::uint8_t>(portal_id));
    Point<std::int16_t> start_pos = physics.get_y_below(spawn_point);

    player.respawn(start_pos, map_info.is_underwater());
    camera.set_position(start_pos);
    camera.set_view(map_info.get_walls(), map_info.get_borders());
}

void Stage::draw(float alpha) const
{
    if (state != ACTIVE) {
        return;
    }

    Point<std::int16_t> viewpos = camera.position(alpha);
    Point<double> viewrpos = camera.realposition(alpha);
    double viewx = viewrpos.x();
    double viewy = viewrpos.y();

    backgrounds.drawbackgrounds(viewx, viewy, alpha);
    for (auto id : Layer::IDs) {
        tiles_objs.draw(id, viewpos, alpha);
        reactors.draw(id, viewx, viewy, alpha);
        npcs.draw(id, viewx, viewy, alpha);
        mobs.draw(id, viewx, viewy, alpha);
        chars.draw(id, viewx, viewy, alpha);
        player.draw(id, viewx, viewy, alpha);
        drops.draw(id, viewx, viewy, alpha);
    }
    combat.draw(viewx, viewy, alpha);
    portals.draw(viewpos, alpha);
    backgrounds.drawforegrounds(viewx, viewy, alpha);
}

void Stage::update()
{
    if (state != ACTIVE) {
        return;
    }

    combat.update();
    backgrounds.update();
    tiles_objs.update();

    reactors.update(physics);
    npcs.update(physics);
    mobs.update(physics);
    chars.update(physics);
    drops.update(physics);
    player.update(physics);

    portals.update(player.get_position());
    camera.update(player.get_position());

    if (player.is_invincible()) {
        return;
    }

    if (std::int32_t oid_id = mobs.find_colliding(player.get_phobj())) {
        if (MobAttack attack = mobs.create_attack(oid_id)) {
            MobAttackResult result = player.damage(attack);
            TakeDamagePacket{result, TakeDamagePacket::TOUCH}.dispatch();
        }
    }
}

void Stage::show_character_effect(std::int32_t cid, CharEffect::Id effect)
{
    if (auto character = get_character(cid)) {
        character->show_effect_id(effect);
    }
}

void Stage::check_portals()
{
    if (player.is_attacking()) {
        return;
    }

    Point<std::int16_t> playerpos = player.get_position();
    Portal::WarpInfo warpinfo = portals.find_warp_at(playerpos);
    if (warpinfo.intramap) {
        Point<std::int16_t> spawnpoint
            = portals.get_portal_by_name(warpinfo.to_name);
        Point<std::int16_t> startpos = physics.get_y_below(spawnpoint);
        player.respawn(startpos, map_info.is_underwater());
    } else if (warpinfo.valid) {
        ChangeMapPacket{false, warpinfo.mapid, warpinfo.name, false}
            .dispatch();
    }
}

void Stage::check_seats()
{
    if (player.is_sitting() || player.is_attacking()) {
        return;
    }

    nullable_ptr<const Seat> seat = map_info.find_seat(player.get_position());
    player.set_seat(seat);
}

void Stage::check_ladders(bool up)
{
    if (player.is_climbing() || player.is_attacking()) {
        return;
    }

    nullable_ptr<const Ladder> ladder
        = map_info.find_ladder(player.get_position(), up);
    player.set_ladder(ladder);
}

void Stage::check_drops()
{
    Point<std::int16_t> playerpos = player.get_position();
    MapDrops::Loot loot = drops.find_loot_at(playerpos);
    if (loot.first) {
        PickupItemPacket(loot.first, loot.second).dispatch();
    }
}

void Stage::send_key(KeyType::Id type, std::int32_t action, bool down)
{
    if (state != ACTIVE || !playable) {
        return;
    }

    switch (type) {
    case KeyType::ACTION:
        if (down) {
            switch (action) {
            case KeyAction::UP:
                check_ladders(true);
                check_portals();
                break;
            case KeyAction::DOWN:
                check_ladders(false);
                break;
            case KeyAction::SIT:
                check_seats();
                break;
            case KeyAction::ATTACK:
                combat.use_move(0);
                break;
            case KeyAction::PICK_UP:
                check_drops();
                break;
            default:
                break;
            }
        }

        playable->send_action(KeyAction::action_by_id(action), down);
        break;
    case KeyType::SKILL:
        combat.use_move(action);
        break;
    case KeyType::ITEM:
        player.use_item(action);
        break;
    case KeyType::FACE:
        player.set_expression(action);
        break;
    default:
        break;
    }
}

Cursor::State Stage::send_cursor(bool pressed, Point<std::int16_t> position)
{
    return npcs.send_cursor(pressed, position, camera.position());
}

bool Stage::is_player(std::int32_t cid) const
{
    return cid == player.get_oid();
}

MapNpcs& Stage::get_npcs()
{
    return npcs;
}

MapChars& Stage::get_chars()
{
    return chars;
}

MapMobs& Stage::get_mobs()
{
    return mobs;
}

MapReactors& Stage::get_reactors()
{
    return reactors;
}

MapDrops& Stage::get_drops()
{
    return drops;
}

Player& Stage::get_player()
{
    return player;
}

Combat& Stage::get_combat()
{
    return combat;
}

nullable_ptr<Char> Stage::get_character(std::int32_t cid)
{
    if (is_player(cid)) {
        return player;
    } else {
        return chars.get_char(cid);
    }
}

std::uint8_t Stage::get_world() const noexcept
{
    return world;
}

void Stage::set_world(std::uint8_t world_id) noexcept
{
    world = world_id;
}

std::uint8_t Stage::get_channel() const noexcept
{
    return channel;
}

void Stage::set_channel(std::uint8_t chan) noexcept
{
    channel = chan;
}

std::uint8_t Stage::get_channel_count() const noexcept
{
    return channel_count;
}

void Stage::set_channel_count(std::uint8_t ch_count) noexcept
{
    channel_count = ch_count;
}
} // namespace jrc
