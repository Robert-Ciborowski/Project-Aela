/*
* Class: 3D Bounding Box
* Author: Robert Ciborowski
* Date: 29/08/2017
* Description: A class used for storing the properties of a 3D bounding box. This class is similar to the Cuboid class
*              but it also stores its vertices, making it possible to perform matrix transformations on the box.
*              Note: the position of the bounding box is the center of it.
*/

#pragma once
#include "../../Utilities/Rect/Cuboid.h"
#include "../Transformable/Transformable3D.h"

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

namespace Aela {
	class BoundingBox3D : public Transformable3D {
		public:
			BoundingBox3D() {

			}

			BoundingBox3D(float x, float y, float z, float width, float height, float depth) {
				position = glm::vec3(x, y, z);
				this->width = width;
				this->height = height;
				this->depth = depth;
			}

			void generateVertices();
			bool verticesWereGenerated();

			// These are getters and setters.
			glm::vec3 getVertex(int id);
			std::string getVerticesAsString();

			void setWidth(float width), setHeight(float height), setDpeth(float depth);
			float getWidth(), getHeight(), getDepth();
			void setValues(float x, float y, float z, float width, float height, float depth);

		private:
			std::vector<glm::vec3> vertices;
			float width = 0, height = 0, depth = 0;
	};
}