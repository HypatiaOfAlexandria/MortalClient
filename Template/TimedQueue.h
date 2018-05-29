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
#include "../Constants.h"

#include <cstdint>
#include <functional>
#include <queue>

namespace jrc
{
template<typename T>
class TimedQueue
{
public:
    TimedQueue(std::function<void(const T&)> in_action)
        : action(in_action), time(0)
    {
    }

    void push(std::int64_t delay, const T& t)
    {
        queue.emplace(time + delay, t);
    }

    template<typename... Args>
    void emplace(std::int64_t delay, Args&&... args)
    {
        queue.emplace(time + delay, std::move(args)...);
    }

    void update(std::int64_t timestep = Constants::TIMESTEP)
    {
        time += timestep;

        for (; !queue.empty(); queue.pop()) {
            const Timed& top = queue.top();
            if (top.when > time) {
                break;
            }

            action(top.value);
        }
    }

private:
    struct Timed {
        T value;
        std::int64_t when;

        Timed(std::int64_t w, const T& v) : value{v}, when{w}
        {
        }

        template<typename... Args>
        Timed(std::int64_t w, Args&&... args)
            : value{std::forward<Args>(args)...}, when{w}
        {
        }
    };

    struct TimedComparator {
        bool operator()(const Timed& a, const Timed& b) const
        {
            return a.when > b.when;
        }
    };

    std::priority_queue<Timed, std::vector<Timed>, TimedComparator> queue;
    std::function<void(const T&)> action;
    std::int64_t time;
};
} // namespace jrc
