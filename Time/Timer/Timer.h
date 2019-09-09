/*
* Class: Timer
* Author: Robert Ciborowski
* Date: 20/03/2018
* Description: A class for timed events.
*/

#pragma once
#include "../Clock.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

namespace Aela {
	class Timer {
		public:
			explicit Timer(Clock* time);

			void update();

			std::string scheduleEventInNanos(long long waitTime, std::function<void()> event);
			std::string scheduleEventInMicros(long long waitTime, std::function<void()> event);
			std::string scheduleEventInMillis(long long waitTime, std::function<void()> event);
			std::string scheduleEventInSeconds(long long waitTime, std::function<void()> event);

			void scheduleEventInNanosUsingTag(std::string tag, long long waitTime, std::function<void()> event);
			void scheduleEventInMicrosUsingTag(std::string tag, long long waitTime, std::function<void()> event);
			void scheduleEventInMillisUsingTag(std::string tag, long long waitTime, std::function<void()> event);
			void scheduleEventInSecondsUsingTag(std::string tag, long long waitTime, std::function<void()> event);

			bool removeEvent(std::string tag);
			void removeAllEvents();
			void endAllEventsNextUpdate();

			void pause();
			void unpause();
			bool isPaused();

		private:
			struct TimedEvent {
				long long time;
				std::function<void()> event;
				std::string tag;
			};

			Clock* time;
			std::vector<TimedEvent> timedEvents;
			bool clearTimedEvents = false;
			bool paused = false;
			unsigned long long pauseTime = 0;
	};
}
