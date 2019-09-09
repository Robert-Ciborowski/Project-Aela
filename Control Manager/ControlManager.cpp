/*
* Class: Control Manager
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class that manages user controls.
*/

#include "ControlManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Error Handler/ErrorHandling.h"
#include <iostream>

using namespace Aela;

// This disables the following compilation warning:
// 'int': forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable : 4800)

void ControlManager::setWindow(Window* setWindow) {
	window = setWindow;
}

void ControlManager::setTime(Clock* setTime) {
	time = setTime;
}

void ControlManager::updateCameraTransforms(Camera3D* camera) {
	// This will only run if the window is focused.
	if (window->isFocused()) {
		windowFocus = true;

		float deltaTime = (float) time->getTimeBetweenFramesInNanos();

		// This gets the cursor's position.
		int xpos, ypos;
		window->getCursorPositionInWindow(&xpos, &ypos);

		// This moves the cursor back to the middle of the window.
		int width, height;
		window->getDimensions(&width, &height);
		window->setCursorPositionInWindow(width / 2, height / 2);

		// This gets the horizontal and vertical angles.
		float horizontalAngle = camera->getRotation()->x;
		float verticalAngle = camera->getRotation()->y;

		// If the renderer is in charge of updating the camera controls (rather than an animator being responsible for doing so),
		// the renderer must update the camera's rotation.
		if (camera->isUsingKeyboardControls()) {
			// This computes the new horizontal angle.
			horizontalAngle += mouseSpeed * float(width / 2 - xpos);

			// This adjusts the horizontal angle so that it stays between 0 and PI * 2.
			if (horizontalAngle >= glm::pi<float>() * 2) {
				horizontalAngle -= glm::pi<float>() * 2;
			}
			if (horizontalAngle <= 0) {
				horizontalAngle += glm::pi<float>() * 2;
			}

			// This computes the new vertical angle.
			float verticalModifier = mouseSpeed * float(height / 2 - ypos);

			// This checks to see if the user is trying to make the camera go upside down by moving the camera up
			// too far (vertical angle of PI/2 in radians). This also allows the camera to go upside down as long as
			// allowUpsideDownCamera is true.
			if ((!allowUpsideDownCamera && verticalModifier > 0 && verticalAngle + verticalModifier <= glm::pi<float>() / 2) || allowUpsideDownCamera) {
				verticalAngle += mouseSpeed * float(height / 2 - ypos);
			} else if (!allowUpsideDownCamera && verticalModifier > 0) {
				verticalAngle = glm::pi<float>() / 2;
			}

			// This checks to see if the user is trying to make the camera go upside down by moving the camera down
			// too far (vertical angle of -PI/2 in radians). This also allows the camera to go upside down as long as
			// allowUpsideDownCamera is true.
			if ((!allowUpsideDownCamera && verticalModifier < 0 && verticalAngle + mouseSpeed * float(height / 2 - ypos) >= glm::pi<float>() / -2) || allowUpsideDownCamera) {
				verticalAngle += mouseSpeed * float(height / 2 - ypos);
			} else if (!allowUpsideDownCamera && verticalModifier < 0) {
				verticalAngle = glm::pi<float>() / -2;
			}

			camera->setProperty(Transformable3DProperty::X_ROTATION, horizontalAngle);
			camera->setProperty(Transformable3DProperty::Y_ROTATION, verticalAngle);
		}

		// This converts the coordinates from rotational to cartesian-planar.
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// These are vectors for the cartesian-plane system.
		glm::vec3 movingRight = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);
		glm::vec3 movingUp = glm::cross(movingRight, direction);

		// This is a position vector.
		glm::vec3 position = *(camera->getPosition());

		if (false) {// keystate[225]) {
			currentSpeed = superSpeed;
		} else {
			currentSpeed = baseSpeed;
		}
		
		// This occurs when 'w' is pressed.
		/*if (keystate[26]) {
			position += direction * deltaTime * currentSpeed;
		}
		// This occurs when 's' is pressed.
		if (keystate[22]) {
			position -= direction * deltaTime * currentSpeed;
		}
		// This occurs when 'd' is pressed.
		if (keystate[7]) {
			position += right * deltaTime * currentSpeed;
		}
		// This occurs when 'a' is pressed.
		if (keystate[4]) {
			position -= right * deltaTime * currentSpeed;
		}

		// This occurs when space is pressed.
		if (keystate[44]) {
			position += straightUp * deltaTime * currentSpeed;
		}

		// This occurs when movingLeft ctrl is pressed.
		if (keystate[224]) {
			position -= straightUp * deltaTime * currentSpeed;
		}*/

		// This sets all of the camera's position and view related properties.
		camera->setPosition(position);
	} else {
		windowFocus = false;
	}
}

void ControlManager::transform3DObject(Transformable3D* object, float speedModifier) {
	if (window->isFocused()) {
		float deltaTime = (float) time->getTimeBetweenFramesInNanos();

		// This is for translation.
		// This occurs when "LEFT" is pressed.
		if (keystate[80]) {
			object->translate(glm::vec3(-deltaTime * currentSpeed * speedModifier, 0, 0));
		}
		// This occurs when "RIGHT" is pressed.
		if (keystate[79]) {
			object->translate(glm::vec3(deltaTime * currentSpeed * speedModifier, 0, 0));
		}
		// This occurs when "RIGHT SHIFT" is pressed.
		if (keystate[229]) {
			object->translate(glm::vec3(0, deltaTime * currentSpeed * speedModifier, 0));
		}
		// This occurs when "RIGHT CTRL" is pressed.
		if (keystate[228]) {
			object->translate(glm::vec3(0, -deltaTime * currentSpeed * speedModifier, 0));
		}
		// This occurs when "DOWN" is pressed.
		if (keystate[81]) {
			object->translate(glm::vec3(0, 0, deltaTime * currentSpeed * speedModifier));
		}
		// This occurs when "UP" is pressed.
		if (keystate[82]) {
			object->translate(glm::vec3(0, 0, -deltaTime * currentSpeed * speedModifier));
		}
		// This occurs when "1" is pressed.
		if (keystate[30]) {
			object->rotate(glm::vec3(-deltaTime * currentSpeed * speedModifier, 0, 0));
		}
		// This occurs when "2" is pressed.
		if (keystate[31]) {
			object->rotate(glm::vec3(deltaTime * currentSpeed * speedModifier, 0, 0));
		}
		// This occurs when "3" is pressed.
		if (keystate[32]) {
			object->rotate(glm::vec3(0, -deltaTime * currentSpeed * speedModifier, 0));
		}
		// This occurs when "4" is pressed.
		if (keystate[33]) {
			object->rotate(glm::vec3(0, deltaTime * currentSpeed * speedModifier, 0));
		}
		// This occurs when "5" is pressed.
		if (keystate[34]) {
			object->rotate(glm::vec3(0, 0, -deltaTime * currentSpeed * speedModifier));
		}
		// This occurs when "6" is pressed.
		if (keystate[35]) {
			object->rotate(glm::vec3(0, 0, deltaTime * currentSpeed * speedModifier));
		}
	}
}

void ControlManager::setCameraUpsideDownAllowance(bool allowUpsideDown) {
	allowUpsideDownCamera = allowUpsideDown;
}

void ControlManager::setProperty(ControlManagerProperty property, float value) {
	switch (property) {
		case ControlManagerProperty::ALLOW_UPSIDE_DOWN_CAMERA:
			// Note: there is apparently a performance warning with forcing a float value to a boolean.
			allowUpsideDownCamera = (bool) value;
			break;
		case ControlManagerProperty::CAMERA_ROTATION_SPEED:
			mouseSpeed = value;
			break;
		case ControlManagerProperty::CAMERA_TRANSLATION_SPEED:
			baseSpeed = value;
			break;
		case ControlManagerProperty::CAMERA_TRANSLATION_SUPER_SPEED:
			superSpeed = value;
			break;
	}
}