/*
* Class: Project Aela's Material Key Frame
* Author: Robert Ciborowski
* Date: 04/03/2018
* Description: A class used by Aela's animator to perform property changes on a material.
*              Yes, the name of the class is kind of silly. But it follows the conventions
*              used by KeyFrame2D and KeyFrame3D (which cannot be named 2DKeyFrame and
*              3DKeyFrame since those start with #s).
*/

#pragma once

#include "../../Animation/KeyFrame.h"
#include "../Materials/Material.h"

namespace Aela {
	class KeyFrameMaterial : public KeyFrame {
		public:
			KeyFrameMaterial();

			KeyFrameType getType();
			virtual void setup();
			virtual void setup(KeyFrame* previousKeyFrame);

			// These are getters and setters.
			void setMaterial(Material* material), setTexture(Texture* texture);
			Material* getMaterial();
			Texture* getTexture();
			void setUseTexture(bool use);

		private:
			// The object is NOT a shared ptr like in a 2D keyframe since menu objects are meant to be allocated to the heap and
			// shared while 3D transformables are meant to be stored inside of a map.
			Material* material = nullptr;

			Texture* texture;

			// Other properties, such as luminance, may go here later.

			// These are booleans which specify whether a property of the object should be modified. Even if not modifying one of these
			// properties in a key frame, it's usually okay to leave it as being used since the animator will just force that value to
			// stay the same throughout the keyframe. Setting one of these values to false is only necessary if something else, such as a
			// KeyedAnimator, needs to modify that value.
			bool useTexture = true;
	};
}