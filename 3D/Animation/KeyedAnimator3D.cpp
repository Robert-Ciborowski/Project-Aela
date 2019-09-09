/*
* Class: Project Aela's Keyed 3D Animator
* Author: Robert Ciborowski
* Date: 20/08/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*              The transformations occur based on the user's key inputs.
*/

#include "KeyedAnimator3D.h"
#include "../../Events/KeyEvent.h"
#include "../../Events/EventConstants.h"

using namespace Aela;

void KeyedAnimator::onEvent(Event* event) {
	KeyEvent* keyEvent = static_cast<KeyEvent*>(event);

	if (window->isFocused()) {
		if (keyEvent->getKeycode() == 225) {
			currentSpeed = superSpeed;
		} else {
			currentSpeed = speed;
		}

		switch (keyEvent->getType()) {
			case EventConstants::KEY_PRESSED:
				if (keyEvent->getKeycode() == SDLK_w) {
					movingForward = true;
				}

				if (keyEvent->getKeycode() == SDLK_s) {
					movingBackward = true;
				}

				if (keyEvent->getKeycode() == SDLK_d) {
					movingRight = true;
				}

				if (keyEvent->getKeycode() == SDLK_a) {
					movingLeft = true;
				}

				if (keyEvent->getKeycode() == SDLK_SPACE) {
					movingUp = true;
				}

				if (keyEvent->getKeycode() == SDLK_LCTRL) {
					movingDown = true;
				}
				break;
			case EventConstants::KEY_RELEASED:
				if (keyEvent->getKeycode() == SDLK_w) {
					movingForward = false;
				}

				if (keyEvent->getKeycode() == SDLK_s) {
					movingBackward = false;
				}

				if (keyEvent->getKeycode() == SDLK_d) {
					movingRight = false;
				}

				if (keyEvent->getKeycode() == SDLK_a) {
					movingLeft = false;
				}

				if (keyEvent->getKeycode() == SDLK_SPACE) {
					movingUp = false;
				}

				if (keyEvent->getKeycode() == SDLK_LCTRL) {
					movingDown = false;
				}
				break;
		}
	}
}

void KeyedAnimator::update() {
	if (time != nullptr) {
		long long deltaTime = time->getTimeBetweenFramesInNanos();
		for (auto pair : transformables) {
			Transformable3D* transformable = pair.second;
			float amount = currentSpeed * deltaTime;

			if (movingForward) {
				*transformable->getPosition() += glm::vec3(0, 0, amount);
			}
			if (movingBackward) {
				*transformable->getPosition() -= glm::vec3(0, 0, amount);
			}
			if (movingRight) {
				*transformable->getPosition() -= glm::vec3(amount, 0, 0);
			}
			if (movingLeft) {
				*transformable->getPosition() += glm::vec3(amount, 0, 0);
			}
			if (movingUp) {
				*transformable->getPosition() += glm::vec3(0, amount, 0);
			}
			if (movingDown) {
				*transformable->getPosition() -= glm::vec3(0, amount, 0);
			}
		}
	} else {
		// The keyed animator must not have been start properly.
	}
}

void KeyedAnimator::setTime(Clock* time) {
	this->time = time;
}

Clock* KeyedAnimator::getClock() {
	return time;
}

void KeyedAnimator::setWindow(Window* window) {
	this->window = window;
}

Window* KeyedAnimator::getWindow() {
	return window;
}

bool KeyedAnimator::addTransformable(int key, Transformable3D* transformable) {
	if (transformables.find(key) == transformables.end()) {
		transformables[key] = transformable;
		return true;
	}
	return false;
}

size_t KeyedAnimator::addTransformable(Transformable3D* transformable) {
	transformables[transformables.size()] = transformable;
	return transformables.size();
}

bool KeyedAnimator::removeTransformable(int key) {
	auto position = transformables.find(key);
	if (position != transformables.end()) {
		transformables.erase(position);
		return true;
	}
	return false;
}
