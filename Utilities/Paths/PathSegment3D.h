/*
* Class: Path Segment 3D
* Author: Robert Ciborowski
* Date: 19/09/2017
* Description: A class used to represent a 3D path segment.
*/

#pragma once
#include "PathSegment.h"
#include <glm\glm.hpp>

namespace Aela {
	class PathSegment3D : PathSegment {
		public:
			PathSegment3D() {

			}

			PathSegment3D(double magnitude, glm::vec3 direction, double speed) : PathSegment(magnitude, speed) {
				this->direction = direction;
			}

			void setDirection(glm::vec3 direction) {
				this->direction = direction;
			}

			glm::vec3 getDirection() {
				return direction;
			}

		private:
			// To use quaternions or not to use quaternions... to be "good" or not to be "good"...
			glm::vec3 direction;
	};
}