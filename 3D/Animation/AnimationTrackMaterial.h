/*
* Class: Project Aela's Material Key Frame List
* Author: Robert Ciborowski
* Date: 04/03/2018
* Description: A class that stores keyframes, which specify a new material to use.
*/

#pragma once
#include "../../Animation/AnimationTrack.h"
#include "KeyFrameMaterial.h"

namespace Aela {
	class AnimationTrackMaterial : public AnimationTrack<KeyFrameMaterial> {
		public:
		AnimationTrackMaterial() {
			std::cout << "Constructor called.\n";
		}
	};
}