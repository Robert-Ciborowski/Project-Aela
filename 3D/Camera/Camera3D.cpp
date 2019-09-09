/*
* Class: 3D Camera
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Project Aela's GLRenderer to store properties of a camera.
*/

#include "Camera3D.h"
#include "../../Utilities/glmut.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "../../Events/EventConstants.h"
#include <glm/gtc/matrix_transform.hpp>
using namespace Aela;

glm::mat4 Camera3D::getViewMatrix() {
	return viewMatrix;
}

glm::mat4 Camera3D::getProjectionMatrix() {
	return projectionMatrix;
}

void Camera3D::setFieldOfView(float setFieldOfView) {
	fieldOfView = setFieldOfView;
}

float Camera3D::getFieldOfView() {
	return fieldOfView;
}

void Camera3D::setInUse(bool inUse) {
	this->inUse = inUse;
}

bool Camera3D::isInUse() {
	return inUse;
}

void Camera3D::useControls(bool usingControls) {
	this->usingControls = usingControls;
}

bool Camera3D::isUsingKeyboardControls() {
	return usingControls;
}

void Camera3D::setForceCursorToMiddle(bool forceCursorToMiddle) {
	this->forceCursorToMiddle = forceCursorToMiddle;
}

bool Camera3D::isForcingCursorToMiddle() {
	return forceCursorToMiddle;
}

void Camera3D::setTime(Clock* time) {
	this->time = time;
}

void Camera3D::setWindow(Window* window) {
	this->window = window;
}

Aela::Camera3D::Camera3D() {
	fieldOfView = 1.0f;
	speed = 0.000000015f;
	superSpeed = 0.000000045f;
	currentSpeed = speed;
	mouseSpeed = 0.003f;
}

void Camera3D::onEvent(Event* event) {
	KeyEvent* keyEvent = dynamic_cast<KeyEvent*>(event);

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
			if (keyEvent->getKeycode() == SDLK_LSHIFT) {
				currentSpeed = superSpeed;
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
			if (keyEvent->getKeycode() == SDLK_LSHIFT) {
				currentSpeed = speed;
			}
			break;
		}
}

void Camera3D::update() {
	if (usingControls && inUse && window->isFocused()) {
		// This gets the cursor's position.
		int xpos, ypos;
		window->getCursorPositionInWindow(&xpos, &ypos);

		// This moves the cursor back to the middle of the window.
		int width, height;
		window->getDimensions(&width, &height);

		if (forceCursorToMiddle) {
			window->setCursorPositionInWindow(width / 2, height / 2);
		}

		// This gets the horizontal and vertical angles.
		float horizontalAngle = rotation.x;
		float verticalAngle = rotation.y;

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
		if ((!allowUpsideDown && verticalModifier > 0 && verticalAngle + verticalModifier <= glm::pi<float>() / 2) || allowUpsideDown) {
			verticalAngle += mouseSpeed * float(height / 2 - ypos);
		} else if (!allowUpsideDown && verticalModifier > 0) {
			verticalAngle = glm::pi<float>() / 2;
		}

		// This checks to see if the user is trying to make the camera go upside down by moving the camera down
		// too far (vertical angle of -PI/2 in radians). This also allows the camera to go upside down as long as
		// allowUpsideDownCamera is true.
		if ((!allowUpsideDown && verticalModifier < 0 && verticalAngle + mouseSpeed * float(height / 2 - ypos) >= glm::pi<float>() / -2) || allowUpsideDown) {
			verticalAngle += mouseSpeed * float(height / 2 - ypos);
		} else if (!allowUpsideDown && verticalModifier < 0) {
			verticalAngle = glm::pi<float>() / -2;
		}

		rotation.x = horizontalAngle;
		rotation.y = verticalAngle;

		float deltaTime = (float) time->getTimeBetweenFramesInNanos();

		if (movingUp) {
			position += straightUp * deltaTime * currentSpeed;
		}

		if (movingDown) {
			position -= straightUp * deltaTime * currentSpeed;
		}

		if (movingLeft) {
			position -= right * deltaTime * currentSpeed;
		}

		if (movingRight) {
			position += right * deltaTime * currentSpeed;
		}

		if (movingForward) {
			position += cartesionalDirection * deltaTime * currentSpeed;
		}

		if (movingBackward) {
			position -= cartesionalDirection * deltaTime * currentSpeed;
		}
	}
}

void Camera3D::setViewMatrix(glm::mat4 setViewMatrix) {
	viewMatrix = setViewMatrix;
}

void Camera3D::setProjectionMatrix(glm::mat4 setProjectionMatrix) {
	projectionMatrix = setProjectionMatrix;
}

void Camera3D::focusAtPointOnPlane(glm::vec3 point, glm::vec3 offset) {
	glm::vec3 angle = glm::normalize(point - position);
	setRotation(angle.x + offset.x, angle.y + offset.y, offset.z);
	forceWithinRange(&rotation, 0, glm::pi<float>() * 2);
}

void Camera3D::focusAtPointOnPlane(glm::vec3 point) {
	glm::vec3 angle = glm::normalize(point - position);
	setRotation(angle.x, angle.y, 0);
}

void Camera3D::focusAtPointOnPlane(float x, float y, float z) {
	glm::vec3 point(x, y, z);
	focusAtPointOnPlane(point);
}

void Camera3D::calculateMatrices(int width, int height) {
	projectionMatrix = glm::perspective(fieldOfView, (float) width / height, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(position, position + cartesionalDirection, up);
}

void Camera3D::calculateCartesionalDirection() {
	cartesionalDirection = glm::vec3(
		cos(rotation.y) * sin(rotation.x),
		sin(rotation.y),
		cos(rotation.y) * cos(rotation.x)
	);
}

void Camera3D::calculateRightVector() {
	right = glm::vec3(
		sin(rotation.x - glm::pi<float>() / 2.0f),
		0,
		cos(rotation.x - glm::pi<float>() / 2.0f)
	);
}

void Camera3D::calculateUpVector() {
	up = glm::cross(right, cartesionalDirection);
}

glm::vec3* Camera3D::getCartesionalDirection() {
	return &cartesionalDirection;
}

glm::vec3* Camera3D::getRightVector() {
	return &right;
}

glm::vec3* Camera3D::getUpVector() {
	return &up;
}

glm::vec3 Camera3D::getPointInFrontOfCamera(float distanceFromCamera) {
	glm::vec3 point = position;
	point += cartesionalDirection * distanceFromCamera;
	return point;
}
