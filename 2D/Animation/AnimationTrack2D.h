/*
* Class: Project Aela's 2D Key Frame List
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class that stores 2D keyframes.
*/

#pragma once

#include <vector>
#include "../../Animation/AnimationTrack.h"
#include "KeyFrame2D.h"

namespace Aela {
	class AnimationTrack2D : public AnimationTrack<KeyFrame2D> {
		public:
			AnimationTrack2D() = default;


		private:
	};
}