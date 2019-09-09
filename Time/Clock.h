/*
* Class: Clock
* Author: Robert Ciborowski
* Date: November 2016
* Description: A header-only class for time management.
*/

#pragma once
#include <chrono>
#include <iostream>

using steady_clock = std::chrono::steady_clock;
using time_point = std::chrono::time_point<steady_clock>;

namespace Aela {
	class Clock {
		public:
			Clock() {
				updateTime();
			}

			void updateTime() {
				// This updates the time and the time difference between the current and the last frame.
				lastTime = currentTime;
				currentTime = steady_clock::now();

				if (!started) {
					started = true;
					deltaTime = 0;
				} else {
					deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
				}
			}

			unsigned long long getCurrentTimeInNanos() {
				return std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime.time_since_epoch()).count();
			}

			unsigned long long getCurrentTimeInMicros() {
				return std::chrono::duration_cast<std::chrono::microseconds>(currentTime.time_since_epoch()).count();
			}

			unsigned long long getCurrentTimeInMillis() {
				return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
			}

			unsigned long long getCurrentTimeInSecs() {
				return std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count();
			}

			unsigned long long getLastFrameTimeInNanos() {
				return std::chrono::duration_cast<std::chrono::nanoseconds>(lastTime.time_since_epoch()).count();
			}

			unsigned long long getLastFrameTimeInMicro() {
				return std::chrono::duration_cast<std::chrono::microseconds>(lastTime.time_since_epoch()).count();
			}

			unsigned long long getLastFrameTimeInMillis() {
				return std::chrono::duration_cast<std::chrono::milliseconds>(lastTime.time_since_epoch()).count();
			}

			unsigned long long getTimeBetweenFramesInNanos() {
				return deltaTime;
			}

			unsigned long long getTimeBetweenFramesInMicros() {
				return deltaTime / 1000;
			}

			unsigned long long getTimeBetweenFramesInMillis() {
				return deltaTime / 1000000;
			}

		private:
			time_point currentTime, lastTime;
			long long deltaTime = 0;
			bool started = false;
	};
}
