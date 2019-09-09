/*
* Class: Path Segment
* Author: Robert Ciborowski
* Date: 19/09/2017
* Description: A class used to represent a velocity alongside a distance (a concept which is famously dubbed as a "Petres Vector").
*/

#pragma once

namespace Aela {
	class PathSegment {
		public:
			PathSegment() {

			}

			PathSegment(double magnitude, double speed) {
				this->magnitude = magnitude;
				this->speed = speed;
			}

			void setMagnitude(double magnitude) {
				this->magnitude = magnitude;
			}


			void setSpeed(double speed) {
				this->speed = speed;
			}

			double getMagnitude() {
				return magnitude;
			}

			double getSpeed() {
				return speed;
			}

		protected:
			double magnitude = 0, speed = 0;
	};
}