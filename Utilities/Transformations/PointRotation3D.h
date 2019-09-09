/*
* Class: Point Rotation
* Author: Robert Ciborowski
* Date: 04/08/2017
* Description: A class used to specify a rotation around a point.
*/

#pragma once

#include <glm/glm.hpp>

namespace Aela {
	class PointRotation3D {
		public:
			PointRotation3D() {

			}

			PointRotation3D(glm::vec3* rotation, glm::vec3* point) {
				this->rotation = *rotation;
				this->point = *point;
			}

			glm::vec3* getRotation();
			void setRotation(glm::vec3* rotation);
			glm::vec3* getPoint();
			void setPoint(glm::vec3* point);

		private:
			glm::vec3 rotation;
			glm::vec3 point;
	};
}