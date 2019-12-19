#pragma once

#include "SDL.h"
#include "../Window/Window.h"
#include "Event.h"
#include "EventListener.h"
#include "EventListenerList.h"
#include <forward_list>
#include <queue>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace Aela {
	class EventDispatcher {
		public:
			EventDispatcher();
			EventDispatcher(const EventDispatcher&);
			~EventDispatcher();

			void runAsync();
			void stopBlocking();
			void stopAsync();

			void fireEvent(Event* event);
			void addListener(int type, EventListener listener);

		private:
			std::atomic<bool> running;

			std::thread eventThread;
			std::condition_variable notifier;
			std::mutex eventMutex;

			std::queue<Event*> eventQueue;
			EventListenerList listeners;

			void dispatchEvents();
			bool waitUntilEvent(Event*& event);
	};
}