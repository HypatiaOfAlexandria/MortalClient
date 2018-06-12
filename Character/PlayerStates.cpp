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
#include "PlayerStates.h"

#include "../Audio/Audio.h"

namespace jrc
{
// Base class
void PlayerState::play_jumpsound() const
{
    Sound(Sound::JUMP).play();
}

// Null state
void PlayerNullState::update_state(Player& player) const
{
    Char::State state;
    if (player.get_phobj().on_ground) {
        if (player.is_key_down(KeyAction::LEFT)) {
            state = Char::WALK;
            player.set_direction(false);
        } else if (player.is_key_down(KeyAction::RIGHT)) {
            state = Char::WALK;
            player.set_direction(true);
        } else if (player.is_key_down(KeyAction::DOWN)) {
            state = Char::PRONE;
        } else {
            state = Char::STAND;
        }
    } else {
        nullable_ptr<const Ladder> ladder = player.get_ladder();
        if (ladder) {
            state = ladder->is_ladder() ? Char::LADDER : Char::ROPE;
        } else {
            state = Char::FALL;
        }
    }

    player.get_phobj().clear_flags();

    player.set_state(state);
}

// Standing
void PlayerStandState::initialize(Player& player) const
{
    player.get_phobj().type = PhysicsObject::NORMAL;
}

void PlayerStandState::send_action(Player& player,
                                   KeyAction::Id ka,
                                   bool down) const
{
    if (player.is_attacking()) {
        return;
    }

    if (down) {
        switch (ka) {
        case KeyAction::LEFT:
            player.set_direction(false);
            player.set_state(Char::WALK);
            break;
        case KeyAction::RIGHT:
            player.set_direction(true);
            player.set_state(Char::WALK);
            break;
        case KeyAction::JUMP:
            play_jumpsound();
            player.get_phobj().v_force = -player.get_jump_force();
            break;
        case KeyAction::DOWN:
            player.set_state(Char::PRONE);
            break;
        default:
            break;
        }
    }
}

void PlayerStandState::update(Player& player) const
{
    if (!player.get_phobj().enable_jd) {
        player.get_phobj().set_flag(PhysicsObject::CHECK_BELOW);
    }
}

void PlayerStandState::update_state(Player& player) const
{
    if (!player.get_phobj().on_ground) {
        player.set_state(Char::FALL);
    }
}

// Walking
void PlayerWalkState::initialize(Player& player) const
{
    player.get_phobj().type = PhysicsObject::NORMAL;
}

void PlayerWalkState::send_action(Player& player,
                                  KeyAction::Id ka,
                                  bool down) const
{
    if (player.is_attacking()) {
        return;
    }

    if (down) {
        switch (ka) {
        case KeyAction::LEFT:
            player.set_direction(false);
            break;
        case KeyAction::RIGHT:
            player.set_direction(true);
            break;
        case KeyAction::JUMP:
            play_jumpsound();
            player.get_phobj().v_force = -player.get_jump_force();
            break;
        case KeyAction::DOWN:
            player.set_state(Char::PRONE);
            break;
        default:
            break;
        }
    }
}

bool PlayerWalkState::haswalkinput(const Player& player) const
{
    return player.is_key_down(KeyAction::LEFT) ||
           player.is_key_down(KeyAction::RIGHT);
}

void PlayerWalkState::update(Player& player) const
{
    if (!player.is_attacking() && haswalkinput(player)) {
        player.get_phobj().h_force += player.get_flip()
                                          ? player.get_walk_force()
                                          : -player.get_walk_force();
    }

    if (!player.get_phobj().enable_jd) {
        player.get_phobj().set_flag(PhysicsObject::CHECK_BELOW);
    }
}

void PlayerWalkState::update_state(Player& player) const
{
    if (player.get_phobj().on_ground) {
        if (!haswalkinput(player) || player.get_phobj().hspeed == 0.0f) {
            player.set_state(Char::STAND);
        }
    } else {
        player.set_state(Char::FALL);
    }
}

// Falling
void PlayerFallState::initialize(Player& player) const
{
    player.get_phobj().type = PhysicsObject::NORMAL;
}

void PlayerFallState::send_action(Player& player,
                                  KeyAction::Id ka,
                                  bool down) const
{
    if (down) {
        switch (ka) {
        case KeyAction::LEFT:
            player.set_direction(false);
            break;
        case KeyAction::RIGHT:
            player.set_direction(true);
            break;
        default:
            break;
        }
    }
}

void PlayerFallState::update(Player& player) const
{
    auto& hspeed = player.get_phobj().hspeed;
    if (player.is_key_down(KeyAction::LEFT) && hspeed > 0.0) {
        hspeed -= 0.025;
        if (hspeed < 0.0) {
            hspeed = 0.0;
        }
    } else if (player.is_key_down(KeyAction::RIGHT) && hspeed < 0.0) {
        hspeed += 0.025;
        if (hspeed > 0.0) {
            hspeed = 0.0;
        }
    }
}

void PlayerFallState::update_state(Player& player) const
{
    if (player.get_phobj().on_ground) {
        if (player.is_key_down(KeyAction::LEFT)) {
            player.set_direction(false);
            player.set_state(Char::WALK);
        } else if (player.is_key_down(KeyAction::RIGHT)) {
            player.set_direction(true);
            player.set_state(Char::WALK);
        } else {
            player.set_state(Char::STAND);
        }
    } else if (player.is_underwater()) {
        player.set_state(Char::SWIM);
    }
}

// Prone
void PlayerProneState::send_action(Player& player,
                                   KeyAction::Id ka,
                                   bool down) const
{
    if (down) {
        switch (ka) {
        case KeyAction::JUMP:
            if (player.get_phobj().enable_jd &&
                player.is_key_down(KeyAction::DOWN)) {
                play_jumpsound();
                player.get_phobj().y = player.get_phobj().ground_below;
                player.set_state(Char::FALL);
            } else {
                player.set_state(Char::STAND);
                player.send_action(ka, down);
            }
            break;
        default:
            break;
        }
    } else {
        switch (ka) {
        case KeyAction::DOWN:
            player.set_state(Char::STAND);
            break;
        default:
            break;
        }
    }
}

void PlayerProneState::update(Player& player) const
{
    if (!player.get_phobj().enable_jd) {
        player.get_phobj().set_flag(PhysicsObject::CHECK_BELOW);
    }
}

// Sitting
void PlayerSitState::send_action(Player& player,
                                 KeyAction::Id ka,
                                 bool down) const
{
    if (down) {
        switch (ka) {
        case KeyAction::LEFT:
            player.set_direction(false);
            player.set_state(Char::WALK);
            break;
        case KeyAction::RIGHT:
            player.set_direction(true);
            player.set_state(Char::WALK);
            break;
        case KeyAction::JUMP:
            play_jumpsound();
            player.set_state(Char::STAND);
            break;
        case KeyAction::UP:
            player.set_state(Char::SWIM);
            break;
        default:
            break;
        }
    }
}

// Flying
void PlayerFlyState::initialize(Player& player) const
{
    player.get_phobj().type = player.is_underwater() ? PhysicsObject::SWIMMING
                                                     : PhysicsObject::FLYING;
}

void PlayerFlyState::send_action(Player& player,
                                 KeyAction::Id ka,
                                 bool down) const
{
    if (down) {
        switch (ka) {
        case KeyAction::LEFT:
            player.set_direction(false);
            break;
        case KeyAction::RIGHT:
            player.set_direction(true);
            break;
        default:
            break;
        }
    }
}

void PlayerFlyState::update(Player& player) const
{
    if (player.is_attacking()) {
        return;
    }

    if (player.is_key_down(KeyAction::LEFT)) {
        player.get_phobj().h_force = -player.get_fly_force();
    } else if (player.is_key_down(KeyAction::RIGHT)) {
        player.get_phobj().h_force = player.get_fly_force();
    }

    if (player.is_key_down(KeyAction::UP)) {
        player.get_phobj().v_force = -player.get_fly_force();
    } else if (player.is_key_down(KeyAction::DOWN)) {
        player.get_phobj().v_force = player.get_fly_force();
    }
}

void PlayerFlyState::update_state(Player& player) const
{
    if (player.get_phobj().on_ground && player.is_underwater()) {
        Char::State state;
        if (player.is_key_down(KeyAction::LEFT)) {
            state = Char::WALK;
            player.set_direction(false);
        } else if (player.is_key_down(KeyAction::RIGHT)) {
            state = Char::WALK;
            player.set_direction(true);
        } else if (player.is_key_down(KeyAction::DOWN)) {
            state = Char::PRONE;
        } else {
            state = Char::STAND;
        }
        player.set_state(state);
    }
}

// Climbing
void PlayerClimbState::initialize(Player& player) const
{
    player.get_phobj().type = PhysicsObject::FIXATED;
}

void PlayerClimbState::send_action(Player& player,
                                   KeyAction::Id ka,
                                   bool down) const
{
    if (down) {
        switch (ka) {
        case KeyAction::JUMP:
            if (player.is_key_down(KeyAction::LEFT)) {
                play_jumpsound();
                player.set_direction(false);
                player.get_phobj().hspeed = -player.get_walk_force() * 8.0;
                player.get_phobj().vspeed = -player.get_jump_force() / 1.5;
                cancel_ladder(player);
            } else if (player.is_key_down(KeyAction::RIGHT)) {
                play_jumpsound();
                player.set_direction(true);
                player.get_phobj().hspeed = player.get_walk_force() * 8.0;
                player.get_phobj().vspeed = -player.get_jump_force() / 1.5;
                cancel_ladder(player);
            }
            break;
        default:
            break;
        }
    }
}

void PlayerClimbState::update(Player& player) const
{
    if (player.is_key_down(KeyAction::UP)) {
        player.get_phobj().vspeed = -player.get_climb_force();
    } else if (player.is_key_down(KeyAction::DOWN)) {
        player.get_phobj().vspeed = player.get_climb_force();
    } else {
        player.get_phobj().vspeed = 0.0;
    }
}

void PlayerClimbState::update_state(Player& player) const
{
    std::int16_t y = player.get_phobj().get_y();
    bool downwards = player.is_key_down(KeyAction::DOWN);
    auto ladder = player.get_ladder();
    if (ladder && ladder->fell_off(y, downwards)) {
        cancel_ladder(player);
    }
}

void PlayerClimbState::cancel_ladder(Player& player) const
{
    player.set_state(Char::FALL);
    player.set_ladder(nullptr);
}
} // namespace jrc
