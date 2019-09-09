/*
* Class: Project Aela's Keyed 3D Animator
* Author: Robert Ciborowski
* Date: 20/08/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*              The transformations occur based on the user's key inputs.
*/

#pragma once
#include <unordered_map>
#include "../../Time/Clock.h"
#include "../../Window/Window.h"
#include "../../Events/KeyEvent.h"
#include "../Transformable/Transformable3D.h"

namespace Aela {
	class KeyedAnimator {
		public:
			KeyedAnimator() {

			}

			// This is triggered on an event.
			void onEvent(Event* event);

			// This should be called in some srt of animation thread or something.
			void update();

			// These are getters and setters.
			void setTime(Clock* time);
			Clock* getClock();
			void setWindow(Window* window);
			Window* getWindow();
			bool addTransformable(int key, Transformable3D* transformable);
			size_t addTransformable(Transformable3D* transformable);
			bool removeTransformable(int key);

		private:
			// These are the objects from Project Aela that the class uses.
			Clock* time;
			Window* window;
			std::unordered_map<size_t, Transformable3D*> transformables;

			// Speed: 0.000000001f is 1 unit per ns.
			float speed = 0.00000003f, superSpeed = 0.00000012f, currentSpeed = 0.0f, mouseSpeed;

			// These store movements.
			bool movingForward, movingBackward, movingLeft, movingRight, movingUp, movingDown;
	};
}