//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
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
#include "Util\Singleton.h"

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "WindowGLFW.h"
#else
#include "WindowD2D.h"
#endif

namespace IO
{
	class Window : public Singleton<Window>
	{
	public:
		bool init(bool fullscreen);
		void update();
		void begin();
		void end();
		void fadeout(float step, void(*fadeproc)());

	private:
#ifdef JOURNEY_USE_OPENGL
		WindowGLFW window;
#else
		WindowD2D window;
#endif
	};
}