/*
* Name: Project Aela's 3D Camera
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Project Aela's GLRenderer to store properties of a camera.
*/

#pragma once
#include <glm/glm.hpp>
#include "../Transformable/Transformable3D.h"
#include "../../Events/EventListener.h"
#include "../../Events/KeyEvent.h"
#include "../../Time/Clock.h"
#include "../../Window/Window.h"

using namespace Aela;

namespace Aela {
	class Camera3D : public Transformable3D, public EventListener {
		public:
			Camera3D();

			// This is triggered on an event.
			void onEvent(Event* event);

			void update();

			// These are getters and setters.
			void setViewMatrix(glm::mat4 setViewMatrix);
			void setProjectionMatrix(glm::mat4 setProjectionMatrix);
			glm::mat4 getViewMatrix(), getProjectionMatrix();
			void setFieldOfView(float setFieldOfView);
			float getFieldOfView();
			void setInUse(bool inUse);
			bool isInUse();
			void useControls(bool useControls);
			bool isUsingKeyboardControls();
			void setForceCursorToMiddle(bool forceCursorToMiddle);
			bool isForcingCursorToMiddle();
			void setTime(Clock* time);
			void setWindow(Window* window);

			// These functions allow the camera to rotate and look at a point that is on its plane.
			void focusAtPointOnPlane(glm::vec3 point, glm::vec3 offset);
			void focusAtPointOnPlane(glm::vec3 point);
			void focusAtPointOnPlane(float x, float y, float z);

			void calculateMatrices(int width, int height);

			// These functions are mainly used by the renderer to update the camera.
			void calculateCartesionalDirection(), calculateRightVector(), calculateUpVector();
			glm::vec3* getCartesionalDirection(), *getRightVector(), *getUpVector();
			glm::vec3 getPointInFrontOfCamera(float distanceFromCamera);

		private:
			Clock* time{};
			Window* window{};

			// These are the camera's matrices.
			glm::mat4 viewMatrix, projectionMatrix;

			// These are used during camera updating.
			glm::vec3 cartesionalDirection, right, up;

			float fieldOfView;

			// This tells the camera whether the user is in control of the camera.
			bool inUse = false;

			// This tells the camera whether it is responsible for updating camera translation. Keep in mind that the animators
			// can also be responsible for transforming the camera and not only the renderer.
			bool usingControls = true;

			// This keeps track of whether the camera should be allowed to be upside down.
			bool allowUpsideDown = false;

			// This tells the camera to keep the cursor in the middle of the window when it updates its camera.
			bool forceCursorToMiddle = true;

			bool forceToUpdate = false;

			// These store movements.
			bool movingForward = false, movingBackward = false, movingLeft = false, movingRight = false, movingUp = false, movingDown = false;

			// Mouse speed: the radians of rotation per pixel of mouse movement 
			float mouseSpeed;

			// Speed: 0.000000001f is 1 unit per ns.
			float speed, superSpeed, currentSpeed;

			// This is used when computing controls.
			const glm::vec3 straightUp = glm::vec3(0, 0.5, 0);
	};
}
