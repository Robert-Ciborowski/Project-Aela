#include "EventListenerList.h"
#include <iostream>

using namespace Aela;

void EventListenerList::put(int type, EventListener listener) {
	auto iter = listeners.find(type);
	if (iter != listeners.end()) {
		(iter->second).push_front(listener);
	} else {
		listeners[type].push_front(listener);
	}
}

void EventListenerList::fire(Event* event) {
	if (event == nullptr) {
		// no event to fire!
		return;
	}

	fireNonNullEvent(event->getType(), event);
}

void EventListenerList::fire(int type, Event* event) {
	if (event == nullptr) {
		// no event to fire!
		return;
	}

	fireNonNullEvent(type, event);
}

void EventListenerList::fireNonNullEvent(int type, Event* event) {
	auto iter = listeners.find(type);
	if (iter == listeners.end()) {
		// listeners don't exist for this type
		return;
	}

	auto listenerList = iter->second;
	for (auto listener : listenerList) {
		if (event->isConsumed()) {
			return;
		}

		// call the listener
		listener(event);
	}
}
