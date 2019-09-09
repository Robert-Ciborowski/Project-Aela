#pragma once

#include "Event.h"
#include "EventListener.h"

#include <forward_list>
#include <unordered_map>
#include <functional>

namespace Aela {
	class EventListenerList {
		public:
			void put(int type, EventListener listener);

			void fire(Event* event);
			void fire(int type, Event* event);
		private:
			std::unordered_map<int, std::forward_list<EventListener>> listeners; // Listeners defined in C++
			//std::unordered_map<int, luabridge::LuaRef> callbacks; // Listeners defined in Lua

			void fireNonNullEvent(int type, Event* event);
	};
}
