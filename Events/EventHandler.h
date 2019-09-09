#pragma once

#include "SDL.h"
#include "../Window/Window.h"
#include "Event.h"
#include "EventListener.h"
#include "EventDispatcher.h"
#include "EventListenerList.h"

namespace Aela {
	class Renderer;
	class EventHandler {
		public:
			EventHandler();
			~EventHandler();

			void start();
			void stop();

			void fireEvent(Event* event);

			void updateSDLEvents();
			void updateWindowEvents();
			void addListener(int type, EventListener listener);

			void bindWindow(Window* window);
			void bindRenderer(Renderer* renderer);

		private:
			SDL_Event event;
			Window* window;
			Renderer* renderer;
			EventDispatcher dispatcher;
	};
}