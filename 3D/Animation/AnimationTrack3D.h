/*
* Class: Project Aela's 3D Key Frame List
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class that stores 3D keyframes.
*/

#pragma once

#include <vector>
#include "../../Animation/AnimationTrack.h"
#include "KeyFrame3D.h"

namespace Aela {
	class AnimationTrack3D : public AnimationTrack<KeyFrame3D> {
		public:
			AnimationTrack3D() {
				// Nothing is here anymore!
			}
	};
}