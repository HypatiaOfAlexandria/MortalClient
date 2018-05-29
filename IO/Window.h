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
#include "../Error.h"
#include "../Template/Singleton.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <string>

namespace jrc
{
class Window : public Singleton<Window>
{
public:
    Window();
    ~Window() override;

    Error init();
    Error initwindow();

    bool not_closed() const;
    void update();
    void begin() const;
    void end() const;
    void fadeout(float step, std::function<void()> fadeprocedure);
    void check_events();

    void setclipboard(const std::string& text) const;
    std::string getclipboard() const;

    void resize(bool in_game) noexcept;
    std::int16_t get_width() const noexcept;
    std::int16_t get_height() const noexcept;

private:
    void updateopc();

    GLFWwindow* glwnd;
    GLFWwindow* context;
    bool fullscreen;
    float opacity;
    float opcstep;
    std::function<void()> fadeprocedure;

    std::int16_t width;
    std::int16_t height;
};
} // namespace jrc
