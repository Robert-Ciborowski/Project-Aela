/*
* Class: Control Manager
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class that manages user controls.
*/

#pragma once
#include <glm/glm.hpp>
#include "../Window/Window.h"
#include "../3D/Camera/Camera3D.h"
#include "../Time/Clock.h"
#include "../Events/Event.h"

namespace Aela {
	// These enums are used to set properties for the control manager.
	enum class ControlManagerProperty {
		ALLOW_UPSIDE_DOWN_CAMERA,
		CAMERA_TRANSLATION_SPEED, CAMERA_TRANSLATION_SUPER_SPEED,
		CAMERA_ROTATION_SPEED
	};

	// This is the Control Manager class.
	class ControlManager {
		public:
		ControlManager() {
			windowFocus = true;
			baseSpeed = 0.003f;
			superSpeed = 0.012f;
			currentSpeed = 0.0f;
			mouseSpeed = 0.005f;
			allowUpsideDownCamera = true;
		}

		// This computes matrices for a Camera3D.
		void updateCameraTransforms(Camera3D* camera);

		// This is a temporary function used for testing. It translates
		// a Transformable.
		void transform3DObject(Transformable3D* object, float speedModifier);

		// These are setters and getters of the control manager.
		void setWindow(Window* setWindow);
		void setTime(Clock* setTime);
		void setCameraUpsideDownAllowance(bool allowUpsdideDown);
		void setProperty(ControlManagerProperty property, float value);
		void updateKeystate(const Uint8* _keystate);

		// Functions that are bound to keys
		void goSuperSpeed();
		void goNormalSpeed();

		// This is a test function for LUA.
		void test();

		private:
		// These are pointers to other Aela classes.
		Clock* time;
		Window* window;
		const Uint8* keystate;

		// This stores the window's state.
		bool windowFocus;

		// Speed: 0.001f is 1 unit per tick.
		float baseSpeed, superSpeed, currentSpeed, mouseSpeed;

		// Keeps track of whether or not the player should be going fast
		bool shouldGoFast;

		// This keeps track of whether the camera shoudl be allowed to be upside down.
		bool allowUpsideDownCamera;

		// This is used when computing controls.
		const glm::vec3 straightUp = glm::vec3(0, 0.5, 0);
	};
}
