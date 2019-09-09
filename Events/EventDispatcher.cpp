#include <iostream>

#include "EventDispatcher.h"
#include "EventConstants.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

#include <memory>

using namespace Aela;

EventDispatcher::EventDispatcher() {
	running = { false };
}

EventDispatcher::~EventDispatcher() {
	stopBlocking();
}

void EventDispatcher::runAsync() {
	running = { true };
	eventThread = std::thread(&EventDispatcher::dispatchEvents, this);
}

void EventDispatcher::stopBlocking() {
	if (eventThread.joinable()) {
		stopAsync();
		eventThread.join();
	}
}

void EventDispatcher::stopAsync() {
	running = { false };
	notifier.notify_one();
}

void EventDispatcher::dispatchEvents() {
	while (true) {
		Event* event;
		if (!waitUntilEvent(event)) {
			break;
		}

		if (event != nullptr) {
			listeners.fire(event);
		}

		delete event;
	}
}

bool EventDispatcher::waitUntilEvent(Event*& event) {
	std::unique_lock<std::mutex> uniqueLock(eventMutex);
	while (eventQueue.empty()) {
		notifier.wait(uniqueLock);

		// make sure we weren't told to exit
		if (!running) {
			return false;
		}
	}

	// remove the event from the queue
	event = eventQueue.front();
	eventQueue.pop();

	return true;
}

void EventDispatcher::fireEvent(Event* event) {
	std::lock_guard<std::mutex> lockGuard(eventMutex);
	eventQueue.push(event);

	notifier.notify_one();
}

void EventDispatcher::addListener(int type, EventListener listener) {
	listeners.put(type, listener);
}
