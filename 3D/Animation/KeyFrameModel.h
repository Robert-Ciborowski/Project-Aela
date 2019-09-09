/*
* Class: Project Aela's Model Key Frame
* Author: Robert Ciborowski
* Date: 04/03/2018
* Description: A class used by Aela's animator to switch the model used by a ModelEntity.
*/

#pragma once

#include "../../Animation/KeyFrame.h"
#include "../Models/Model.h"
#include "../Models/ModelEntity.h"

namespace Aela {
	class KeyFrameModel : public KeyFrame {
	public:
		KeyFrameModel() {

		}

		KeyFrameType getType();
		virtual void setup();
		virtual void setup(KeyFrame* previousKeyFrame) override;

		// These are getters and setters.
		void setModelEntity(ModelEntity* modelEntity), setModel(Model* model);
		ModelEntity* getModelEntity();
		Model* getModel();
		void setUseModel(bool use);

	private:
		// The object is NOT a shared ptr like in a 2D keyframe since menu objects are meant to be allocated to the heap and
		// shared while 3D transformables are meant to be stored inside of a map.
		ModelEntity* modelEntity = nullptr;

		Model* model;

		// Other properties, may go here later. However, I don't know what other properties would even go here.

		// These are booleans which specify whether a property of the object should be modified. Even if not modifying one of these
		// properties in a key frame, it's usually okay to leave it as being used since the animator will just force that value to
		// stay the same throughout the keyframe. Setting one of these values to false is only necessary if something else, such as a
		// KeyedAnimator, needs to modify that value.
		bool useModel = true;
	};
}