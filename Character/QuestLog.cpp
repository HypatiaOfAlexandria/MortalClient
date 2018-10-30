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
#include "QuestLog.h"

namespace jrc
{
void Questlog::add_started(std::int16_t qid, std::string&& qdata)
{
    started[qid] = std::move(qdata);
}

void Questlog::add_in_progress(std::int16_t qid,
                               std::int16_t qidl,
                               std::string&& qdata)
{
    in_progress[qid] = {qidl, std::move(qdata)};
}

void Questlog::add_completed(std::int16_t qid, std::int64_t time)
{
    completed[qid] = time;
}

bool Questlog::is_started(std::int16_t qid)
{
    return started.count(qid) > 0;
}

std::int16_t Questlog::get_last_started()
{
    auto qend = started.end();
    --qend;
    return qend->first;
}
} // namespace jrc
