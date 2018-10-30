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
#include "Skill.h"

#include "../../Character/SkillId.h"
#include "../../Data/SkillData.h"
#include "../../Util/Misc.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

#include <cstring>

namespace jrc
{
Skill::Skill(std::int32_t id) : skillid(id)
{
    const SkillData& data = SkillData::get(skillid);

    std::string str_id = skillid < 10000000
                             ? string_format::extend_id(skillid, 7)
                             : std::to_string(skillid);
    char img_str_buf[7];
    std::memcpy(img_str_buf, str_id.data(), 3);
    std::memcpy(img_str_buf + 3, ".img", 4);
    nl::node src
        = nl::nx::skill[std::string_view{img_str_buf, 7}]["skill"][str_id];

    projectile = true;
    overregular = false;

    sound = std::make_unique<SingleSkillSound>(str_id);

    bool by_level_effect = src["CharLevel"]["10"]["effect"].size() > 0;
    bool multi_effect = src["effect0"].size() > 0;
    if (by_level_effect) {
        useeffect = std::make_unique<ByLevelUseEffect>(src);
    } else if (multi_effect) {
        useeffect = std::make_unique<MultiUseEffect>(src);
    } else {
        bool is_animation
            = src["effect"]["0"].data_type() == nl::node::type::bitmap;
        bool has_effect1 = src["effect"]["1"].size() > 0;
        if (is_animation) {
            useeffect = std::make_unique<SingleUseEffect>(src);
        } else if (has_effect1) {
            useeffect = std::make_unique<TwoHUseEffect>(src);
        } else {
            switch (skillid) {
            case SkillId::IRON_BODY:
            case SkillId::MAGIC_ARMOR:
                useeffect = std::make_unique<IronBodyUseEffect>();
                break;
            default:
                useeffect = std::make_unique<NoUseEffect>();
            }
        }
    }

    bool bylevelhit = src["CharLevel"]["10"]["hit"].size() > 0;
    bool byskilllevelhit = src["level"]["1"]["hit"].size() > 0;
    bool hashit0 = src["hit"]["0"].size() > 0;
    bool hashit1 = src["hit"]["1"].size() > 0;
    if (bylevelhit) {
        if (hashit0 && hashit1) {
            hiteffect = std::make_unique<ByLevelTwoHHitEffect>(src);
        } else {
            hiteffect = std::make_unique<ByLevelHitEffect>(src);
        }
    } else if (byskilllevelhit) {
        hiteffect = std::make_unique<BySkillLevelHitEffect>(src);
    } else if (hashit0 && hashit1) {
        hiteffect = std::make_unique<TwoHHitEffect>(src);
    } else if (hashit0) {
        hiteffect = std::make_unique<SingleHitEffect>(src);
    } else {
        hiteffect = std::make_unique<NoHitEffect>();
    }

    bool hasaction0 = src["action"]["0"].data_type() == nl::node::type::string;
    bool hasaction1 = src["action"]["1"].data_type() == nl::node::type::string;
    if (hasaction0 && hasaction1) {
        action = std::make_unique<TwoHAction>(src);
    } else if (hasaction0) {
        action = std::make_unique<SingleAction>(src);
    } else if (data.is_attack()) {
        bool bylevel = src["level"]["1"]["action"].data_type()
                       == nl::node::type::string;
        if (bylevel) {
            action = std::make_unique<ByLevelAction>(src, skillid);
        } else {
            action = std::make_unique<RegularAction>();
            overregular = true;
        }
    } else {
        action = std::make_unique<NoAction>();
    }

    bool hasball = src["ball"].size() > 0;
    bool bylevelball = src["level"]["1"]["ball"].size() > 0;
    if (bylevelball) {
        bullet = std::make_unique<BySkillLevelBullet>(src, skillid);
    } else if (hasball) {
        bullet = std::make_unique<SingleBullet>(src);
    } else {
        bullet = std::make_unique<RegularBullet>();
        projectile = false;
    }
}

void Skill::apply_useeffects(Char& user) const
{
    useeffect->apply(user);

    sound->play_use();
}

void Skill::apply_actions(Char& user, Attack::Type type) const
{
    action->apply(user, type);
}

void Skill::apply_stats(const Char& user, Attack& attack) const
{
    attack.skill = skillid;

    std::int32_t level = user.get_skill_level(skillid);
    const SkillData::Stats stats = SkillData::get(skillid).get_stats(level);

    if (stats.fixdamage) {
        attack.fix_damage = stats.fixdamage;
        attack.damage_type = Attack::DMG_FIXED;
    } else if (stats.matk) {
        attack.matk += stats.matk;
        attack.damage_type = Attack::DMG_MAGIC;
    } else {
        attack.min_damage *= stats.damage;
        attack.max_damage *= stats.damage;
        attack.damage_type = Attack::DMG_WEAPON;
    }
    attack.critical += stats.critical;
    attack.ignore_def += stats.ignoredef;
    attack.mob_count = stats.mobcount;
    attack.h_range = stats.hrange;

    switch (attack.type) {
    case Attack::RANGED:
        attack.hit_count = stats.bulletcount;
        break;
    default:
        attack.hit_count = stats.attackcount;
    }

    if (!stats.range.empty())
        attack.range = stats.range;

    if (projectile && !attack.bullet) {
        switch (skillid) {
        case SkillId::THREE_SNAILS:
            switch (level) {
            case 1:
                attack.bullet = 4000019;
                break;
            case 2:
                attack.bullet = 4000000;
                break;
            case 3:
                attack.bullet = 4000016;
                break;
            }
            break;
        default:
            attack.bullet = skillid;
        }
    }

    if (overregular) {
        attack.stance = user.get_look().get_stance();
        if (attack.type == Attack::CLOSE && !projectile) {
            attack.range = user.get_afterimage().get_range();
        }
    }
}

void Skill::apply_hit_effects(const AttackUser& user, Mob& target) const
{
    hiteffect->apply(user, target);

    sound->play_hit();
}

Animation Skill::get_bullet(const Char& user, std::int32_t bulletid) const
{
    return bullet->get(user, bulletid);
}

bool Skill::is_attack() const
{
    return SkillData::get(skillid).is_attack();
}

bool Skill::is_skill() const
{
    return true;
}

std::int32_t Skill::get_id() const
{
    return skillid;
}

SpecialMove::ForbidReason Skill::can_use(std::int32_t level,
                                         Weapon::Type weapon,
                                         const Job& job,
                                         std::uint16_t hp,
                                         std::uint16_t mp,
                                         std::uint16_t bullets) const
{
    if (level <= 0 || level > SkillData::get(skillid).get_master_level())
        return FBR_OTHER;

    if (!job.can_use(skillid))
        return FBR_OTHER;

    const SkillData::Stats stats = SkillData::get(skillid).get_stats(level);

    if (hp <= stats.hpcost)
        return FBR_HPCOST;

    if (mp < stats.mpcost)
        return FBR_MPCOST;

    Weapon::Type reqweapon = SkillData::get(skillid).get_required_weapon();
    if (weapon != reqweapon && reqweapon != Weapon::NONE) {
        return FBR_WEAPONTYPE;
    }

    switch (weapon) {
    case Weapon::BOW:
    case Weapon::CROSSBOW:
    case Weapon::CLAW:
    case Weapon::GUN:
        return (bullets >= stats.bulletcost) ? FBR_NONE : FBR_BULLETCOST;
    default:
        return FBR_NONE;
    }
}
} // namespace jrc
