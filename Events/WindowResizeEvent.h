/*
 * Name: WindowResizeEvent
 * Author: Robert Ciborowski
 * Date: 13/03/2018
 * Description: An event that is fired when the window is resized.
 */

#pragma once
#include "Event.h"
#include "../Window/Window.h"

namespace Aela {
	class WindowResizeEvent : public Event {
		public:
			WindowResizeEvent(Window* window);
			virtual ~WindowResizeEvent();
			
			Window* getWindow();

		private:
			Window* window;
	};
}