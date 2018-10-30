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
#include "FootholdTree.h"

#include "../../Console.h"

namespace jrc
{
Footholdtree::Footholdtree(nl::node src)
{
    std::int16_t leftw = 30000;
    std::int16_t rightw = -30000;
    std::int16_t botb = -30000;
    std::int16_t topb = 30000;

    for (const auto& basef : src) {
        std::uint8_t layer;
        try {
            layer = static_cast<std::uint8_t>(std::stoi(basef.name()));
        } catch (const std::exception& ex) {
            Console::get().print(__func__, ex.what());
            continue;
        }

        for (const auto& midf : basef) {
            for (const auto& lastf : midf) {
                std::uint16_t id;
                try {
                    id = static_cast<std::uint16_t>(std::stoi(lastf.name()));
                } catch (const std::exception& ex) {
                    Console::get().print(__func__, ex.what());
                    continue;
                }

                const Foothold& foothold
                    = footholds
                          .emplace(std::piecewise_construct,
                                   std::forward_as_tuple(id),
                                   std::forward_as_tuple(lastf, id, layer))
                          .first->second;

                if (foothold.l() < leftw) {
                    leftw = foothold.l();
                }

                if (foothold.r() > rightw) {
                    rightw = foothold.r();
                }

                if (foothold.b() > botb) {
                    botb = foothold.b();
                }

                if (foothold.t() < topb) {
                    topb = foothold.t();
                }

                if (foothold.is_wall()) {
                    continue;
                }

                std::int16_t start = foothold.l();
                std::int16_t end = foothold.r();
                for (std::int16_t i = start; i <= end; ++i) {
                    footholdsbyx.emplace(i, id);
                }
            }
        }
    }

    walls = {leftw + 25, rightw - 25};
    borders = {topb - 300, botb + 100};
}

Footholdtree::Footholdtree() = default;

void Footholdtree::limit_movement(PhysicsObject& phobj) const
{
    if (phobj.h_mobile()) {
        double crnt_x = phobj.crnt_x();
        double next_x = phobj.next_x();

        bool left = phobj.hspeed < 0.0f;
        double wall = get_wall(phobj.fh_id, left, phobj.next_y());
        bool collision = left ? crnt_x >= wall && next_x <= wall
                              : crnt_x <= wall && next_x >= wall;

        if (!collision && phobj.is_flag_set(PhysicsObject::TURN_AT_EDGES)) {
            wall = get_edge(phobj.fh_id, left);
            collision = left ? crnt_x >= wall && next_x <= wall
                             : crnt_x <= wall && next_x >= wall;
        }

        if (collision) {
            phobj.limit_x(wall);
            phobj.clear_flag(PhysicsObject::TURN_AT_EDGES);
        }
    }

    if (phobj.v_mobile()) {
        double crnt_y = phobj.crnt_y();
        double next_y = phobj.next_y();

        auto ground
            = Range<double>(get_fh(phobj.fh_id).ground_below(phobj.crnt_x()),
                            get_fh(phobj.fh_id).ground_below(phobj.next_x()));
        bool collision = crnt_y <= ground.first() && next_y >= ground.second();
        if (collision) {
            phobj.limit_y(ground.second());

            limit_movement(phobj);
        } else {
            if (next_y < borders.first()) {
                phobj.limit_y(borders.first());
            } else if (next_y > borders.second()) {
                phobj.limit_y(borders.second());
            }
        }
    }
}

void Footholdtree::update_fh(PhysicsObject& phobj) const
{
    if (phobj.type == PhysicsObject::FIXATED && phobj.fh_id > 0) {
        return;
    }

    const Foothold& curfh = get_fh(phobj.fh_id);
    bool checkslope = false;

    double x = phobj.crnt_x();
    double y = phobj.crnt_y();
    if (phobj.on_ground) {
        if (std::floor(x) > curfh.r()) {
            phobj.fh_id = curfh.next();
        } else if (std::ceil(x) < curfh.l()) {
            phobj.fh_id = curfh.prev();
        }

        if (phobj.fh_id == 0) {
            phobj.fh_id = get_fhid_below(x, y);
        } else {
            checkslope = true;
        }
    } else {
        phobj.fh_id = get_fhid_below(x, y);
    }

    const Foothold& nextfh = get_fh(phobj.fh_id);
    phobj.fh_slope = nextfh.slope();

    double ground = nextfh.ground_below(x);
    if (phobj.vspeed == 0.0 && checkslope) {
        double vdelta = std::abs(phobj.fh_slope);
        if (phobj.fh_slope < 0.0) {
            vdelta *= (ground - y);
        } else if (phobj.fh_slope > 0.0) {
            vdelta *= (y - ground);
        }

        if (curfh.slope() != 0.0 || nextfh.slope() != 0.0) {
            if (phobj.hspeed > 0.0 && vdelta <= phobj.hspeed) {
                phobj.y = ground;
            } else if (phobj.hspeed < 0.0 && vdelta >= phobj.hspeed) {
                phobj.y = ground;
            }
        }
    }

    phobj.on_ground = phobj.y == ground;

    if (phobj.enable_jd || phobj.is_flag_set(PhysicsObject::CHECK_BELOW)) {
        std::uint16_t belowid
            = get_fhid_below(x, nextfh.ground_below(x) + 1.0);
        if (belowid > 0) {
            double nextground = get_fh(belowid).ground_below(x);
            phobj.enable_jd = (nextground - ground) < 600.0;
            phobj.ground_below = ground + 1.0;
        } else {
            phobj.enable_jd = false;
        }

        phobj.clear_flag(PhysicsObject::CHECK_BELOW);
    }

    if (phobj.fh_layer == 0 || phobj.on_ground) {
        phobj.fh_layer = nextfh.layer();
    }

    if (phobj.fh_id == 0) {
        phobj.fh_id = curfh.id();
        phobj.limit_x(curfh.x1());
    }
}

const Foothold& Footholdtree::get_fh(std::uint16_t fhid) const
{
    auto iter = footholds.find(fhid);
    if (iter == footholds.end()) {
        return nullfh;
    }

    return iter->second;
}

double Footholdtree::get_wall(std::uint16_t curid, bool left, double fy) const
{
    auto shorty = static_cast<std::int16_t>(fy);
    Range<std::int16_t> vertical(shorty - 50, shorty - 1);
    const Foothold& cur = get_fh(curid);
    if (left) {
        const Foothold& prev = get_fh(cur.prev());
        if (prev.is_blocking(vertical)) {
            return cur.l();
        }

        const Foothold& prev_prev = get_fh(prev.prev());
        if (prev_prev.is_blocking(vertical)) {
            return prev.l();
        }
        return walls.first();
    } else {
        const Foothold& next = get_fh(cur.next());
        if (next.is_blocking(vertical)) {
            return cur.r();
        }

        const Foothold& next_next = get_fh(next.next());
        if (next_next.is_blocking(vertical)) {
            return next.r();
        }
        return walls.second();
    }
}

double Footholdtree::get_edge(std::uint16_t curid, bool left) const
{
    const Foothold& fh = get_fh(curid);
    if (left) {
        std::uint16_t previd = fh.prev();
        if (!previd) {
            return fh.l();
        }

        const Foothold& prev = get_fh(previd);
        std::uint16_t prev_previd = prev.prev();
        if (!prev_previd) {
            return prev.l();
        }

        return walls.first();
    } else {
        std::uint16_t nextid = fh.next();
        if (!nextid) {
            return fh.r();
        }

        const Foothold& next = get_fh(nextid);
        std::uint16_t next_nextid = next.next();
        if (!next_nextid) {
            return next.r();
        }

        return walls.second();
    }
}

std::uint16_t Footholdtree::get_fhid_below(double fx, double fy) const
{
    std::uint16_t ret = 0;
    double comp = borders.second();

    auto x = static_cast<std::int16_t>(fx);
    auto range = footholdsbyx.equal_range(x);
    for (auto iter = range.first; iter != range.second; ++iter) {
        const Foothold& fh = footholds.at(iter->second);
        double ycomp = fh.ground_below(fx);
        if (comp >= ycomp && ycomp >= fy) {
            comp = ycomp;
            ret = fh.id();
        }
    }

    return ret;
}

std::int16_t Footholdtree::get_y_below(Point<std::int16_t> position) const
{
    if (std::uint16_t fhid = get_fhid_below(position.x(), position.y())) {
        const Foothold& fh = get_fh(fhid);
        return static_cast<std::int16_t>(fh.ground_below(position.x()));
    } else {
        return borders.second();
    }
}

Range<std::int16_t> Footholdtree::get_walls() const
{
    return walls;
}

Range<std::int16_t> Footholdtree::get_borders() const
{
    return borders;
}
} // namespace jrc
