/*
* Class: Path Segment 2D
* Author: Robert Ciborowski
* Date: 19/09/2017
* Description: A class used to represent a 2D path segment.
*/

#pragma once
#include "PathSegment.h"

namespace Aela {
	class PathSegment2D : PathSegment {
		public:
			PathSegment2D() {

			}

			PathSegment2D(double magnitude, double direction, double speed) : PathSegment(magnitude, speed) {
				this->direction = direction;
			}

			void setDirection(double direction) {
				this->direction = direction;
			}

			double getDirection() {
				return direction;
			}

		private:
			double direction = 0;
	};
}