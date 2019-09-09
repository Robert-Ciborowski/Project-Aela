#include "WindowResizeEvent.h"
#include "EventConstants.h"

using namespace Aela;

Aela::WindowResizeEvent::WindowResizeEvent(Window* window) : Event(EventConstants::WINDOW_RESIZE) {
	this->window = window;
}

Aela::WindowResizeEvent::~WindowResizeEvent() {}

Window * Aela::WindowResizeEvent::getWindow() {
	return nullptr;
}
