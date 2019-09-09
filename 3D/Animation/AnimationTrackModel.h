/*
* Class: Project Aela's Model Key Frame List
* Author: Robert Ciborowski
* Date: 04/03/2018
* Description: A class that stores keyframes, which specify a new model to use for a ModelEntity.
*/

#pragma once
#include "../../Animation/AnimationTrack.h"
#include "KeyFrameModel.h"

namespace Aela {
	class AnimationTrackModel : public AnimationTrack<KeyFrameModel> {
	public:
		AnimationTrackModel() {}
	};
}