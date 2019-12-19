#include <iostream>
#include "../Renderer/Renderer.h"
#include "EventHandler.h"
#include "EventConstants.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "WindowResizeEvent.h"

using namespace Aela;

EventHandler::EventHandler() {
}

EventHandler::~EventHandler() {
	// dispatcher's destructor will be called to verify thread stops, no need to do anything
}

void EventHandler::start() {
	dispatcher.runAsync();
}

void EventHandler::stop() {
	dispatcher.stopBlocking();
}

void Aela::EventHandler::fireEvent(Event* event) {
	dispatcher.fireEvent(event);
}

void EventHandler::updateSDLEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				dispatcher.stopAsync();
				window->quit();
				break;
			case SDL_WINDOWEVENT:
				window->processSDLWindowEvent(event);
				break;
			case SDL_KEYDOWN:
				dispatcher.fireEvent(new KeyEvent(EventConstants::KEY_PRESSED, event.key.keysym.sym, event.key.keysym.mod));
				break;
			case SDL_KEYUP:
				dispatcher.fireEvent(new KeyEvent(EventConstants::KEY_RELEASED, event.key.keysym.sym, event.key.keysym.mod));
				break;
			case SDL_MOUSEBUTTONDOWN: {
				Rect<int>* output = renderer->getOutputToWindow();
				dispatcher.fireEvent(new MouseEvent(EventConstants::MOUSE_PRESSED,
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y,
											event.button.x - output->getX(),
											event.button.y - output->getY(),
											event.wheel.y));
				break;
			}
			case SDL_MOUSEBUTTONUP: {
				Rect<int>* output = renderer->getOutputToWindow();
				dispatcher.fireEvent(new MouseEvent(EventConstants::MOUSE_RELEASED, 
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y,
											event.button.x - output->getX(),
											event.button.y - output->getY(),
											event.wheel.y));
				break;
			}
			case SDL_MOUSEMOTION: {
				Rect<int>* output = renderer->getOutputToWindow();
				dispatcher.fireEvent(new MouseEvent(EventConstants::MOUSE_MOTIONED,
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y,
											event.button.x - output->getX(),
											event.button.y - output->getY(),
											event.wheel.y));
				break;
			}
			case SDL_MOUSEWHEEL: {
				Rect<int>* output = renderer->getOutputToWindow();
				dispatcher.fireEvent(new MouseEvent(EventConstants::MOUSE_WHEEL,
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y,
											event.button.x - output->getX(),
											event.button.y - output->getY(),
											event.wheel.y));
				break;
			}
		}
	}
}

void Aela::EventHandler::updateWindowEvents() {
	// Since Window cannot tell the handler to fire events inside of itself (cause Window and EventHandler
	// would have to include on another), this exists. Someone change our event framework, please.
	if (window->wasResized()) {
		dispatcher.fireEvent(new WindowResizeEvent(window));
	}
}

void EventHandler::addListener(int type, EventListener listener) {
	dispatcher.addListener(type, listener);
}

void EventHandler::bindWindow(Window* window) {
	this->window = window;
}

void EventHandler::bindRenderer(Renderer* renderer) {
	this->renderer = renderer;
}
