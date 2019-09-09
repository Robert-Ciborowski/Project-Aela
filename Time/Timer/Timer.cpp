#include <utility>
#include "Timer.h"

using namespace Aela;

Timer::Timer(Clock* time) {
	this->time = time;
}

std::string Aela::Timer::scheduleEventInNanos(long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime - pauseTime;
	std::string tag = std::to_string((size_t) &event) + "_" + std::to_string(timeOfEvent);
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
	return tag;
}

std::string Aela::Timer::scheduleEventInMicros(long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000 - pauseTime;
	std::string tag = std::to_string((size_t) &event) + "_" + std::to_string(timeOfEvent);
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
	return tag;
}

std::string Timer::scheduleEventInMillis(long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000000 - pauseTime;
	std::string tag = std::to_string((size_t) &event) + "_" + std::to_string(timeOfEvent);
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
	return tag;
}

std::string Aela::Timer::scheduleEventInSeconds(long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000000000 - pauseTime;
	std::string tag = std::to_string((size_t) &event) + "_" + std::to_string(timeOfEvent);
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
	return tag;
}

void Aela::Timer::scheduleEventInNanosUsingTag(std::string tag, long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime - pauseTime;
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = std::move(event);
	timedEvent.tag = std::move(tag);
	timedEvents.push_back(timedEvent);
}

void Aela::Timer::scheduleEventInMicrosUsingTag(std::string tag, long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000 - pauseTime;
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = std::move(event);
	timedEvent.tag = std::move(tag);
	timedEvents.push_back(timedEvent);
}

void Aela::Timer::scheduleEventInMillisUsingTag(std::string tag, long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000000 - pauseTime;
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = std::move(event);
	timedEvent.tag = std::move(tag);
	timedEvents.push_back(timedEvent);
}

void Aela::Timer::scheduleEventInSecondsUsingTag(std::string tag, long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000000000 - pauseTime;
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = std::move(event);
	timedEvent.tag = std::move(tag);
	timedEvents.push_back(timedEvent);
}

bool Aela::Timer::removeEvent(std::string tag) {
	for (size_t i = 0; i < timedEvents.size(); i++) {
		if (timedEvents[i].tag == tag) {
			timedEvents.erase(timedEvents.begin() + i);
			return true;
		}
	}

	return false;
}

void Aela::Timer::removeAllEvents() {
	timedEvents.clear();
}

void Timer::endAllEventsNextUpdate() {
	clearTimedEvents = true;
}

void Timer::pause() {
	paused = true;
}

void Timer::unpause() {
	paused = false;
}

bool Timer::isPaused() {
	return paused;
}

void Timer::update() {
	if (paused) {
		pauseTime += time->getTimeBetweenFramesInNanos();
		return;
	}

	unsigned long long currentTime = time->getCurrentTimeInNanos();

	for (size_t i = 0; i < timedEvents.size(); i++) {
		TimedEvent event = timedEvents[i];

		if (event.time + pauseTime <= currentTime) {
			event.event();
			timedEvents.erase(timedEvents.begin() + i);
			i--;
		}
	}

	if (clearTimedEvents) {
		for (auto e : timedEvents) {
			e.event();
		}

		removeAllEvents();
		clearTimedEvents = false;
	}
}
