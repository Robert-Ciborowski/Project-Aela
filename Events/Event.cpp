#include "Event.h"

using namespace Aela;

Event::Event(int _type) : type(_type) {
}

Event::~Event() {

}

void Event::consume() {
	consumed = true;
}

int Event::getType() {
	return type;
}

bool Event::isConsumed() {
	return consumed;
}
