/*
* Class: Project Aela's 3D Key Frame
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's animator to perform transformations on 3D objects.
*/

#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "../../Animation/KeyFrame.h"
#include "../Transformable/Transformable3D.h"
#include "../../Utilities/Transformations/PointRotation3D.h"

namespace Aela {
	class KeyFrame3D : public KeyFrame {
		public:
			KeyFrame3D() {

			}

			KeyFrameType getType();
			virtual void setup();
			virtual void setup(KeyFrame* previousKeyFrame) override;

			// These are getters and setters.
			void setObject(Transformable3D* object);
			void setTranslation(glm::vec3* position), setTranslation(float x, float y, float z);
			void setRotation(glm::vec3* rotation), setRotation(float x, float y, float z);
			void setPointRotation(PointRotation3D* pointRotation);
			void setScaling(glm::vec3* scaling), setScaling(float x, float y, float z);
			Transformable3D* getObject();
			glm::vec3* getPosition(), *getRotation(), *getScaling(), *getOriginalPosition(), *getOriginalRotation(), *getOriginalScaling();
			PointRotation3D* getPointRotation(), *getOriginalPointRotation();
			void setUseTranslation(bool use), setUseRotation(bool use), setUsePointRotation(bool use), setUseScaling(bool use);
			bool isUsingTranslation(), isUsingRotation(), isUsingPointRotation(), isUsingScaling();

		private:
			// When using 3D transformations on a Transformable3D, make sure that you set object to a value first! Also,
			// if you do not need to use one of the transformations (such as if you do not need to modify the rotation of
			// the object), just don't set that transformation to a value and it will not have an effect on the object due
			// to the way it is auto-initialised.

			// The object is NOT a shared ptr like in a 2D keyframe since menu objects are meant to be allocated to the heap and
			// shared while 3D transformables are meant to be stored inside of a map.
			Transformable3D* object = nullptr;
			glm::vec3 position, originalPosition;
			glm::vec3 rotation, originalRotation;
			PointRotation3D pointRotation, originalPointRotation;
			glm::vec3 scaling = glm::vec3(1), originalScaling;

			// These are booleans which specify whether a property of the object should be modified. Even if not modifying one of these
			// properties in a key frame, it's usually okay to leave it as being used since the animator will just force that value to
			// stay the same throughout the keyframe. Setting one of these values to false is only necessary if something else, such as a
			// KeyedAnimator, needs to modify that value.
			bool useTranslation = true, useRotation = true, usePointRotation = true, useScaling = true;
	};
}