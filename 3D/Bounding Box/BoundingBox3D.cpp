/*
* Class: 3D Bounding Box
* Author: Robert Ciborowski
* Date: 29/08/2017
* Description: A class used for storing the properties of a 3D bounding box. This class is similar to the Cuboid class
*              but it also stores its vertices, making it possible to perform matrix transformations on the box.
*              Note: the position of the bounding box is the center of it.
*/

#include "BoundingBox3D.h"
#include "../../Utilities/strut.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>

using namespace Aela;

void BoundingBox3D::generateVertices() {
	vertices.clear();

	// This sets up the vertices before doing any matrix calculations.
	for (unsigned int i = 0; i < 8; i++) {
		glm::vec3 pos;
		if (i % 4 < 2) {
			pos.x = position.x - width / 2.0f;
		} else {
			pos.x = position.x + width / 2.0f;
		}
		if (i < 4) {
			pos.y = position.y - height / 2.0f;
		} else {
			pos.y = position.y + height / 2.0f;
		}
		if (i % 4 == 1 || i % 4 == 2) {
			pos.z = position.z + depth / 2.0f;
		} else {
			pos.z = position.z - depth / 2.0f;
		}
		std::cout << vertices.size() << " ";
		vertices.push_back(pos);
		std::cout << vertices.size() << " ";
	}

	// This does matrix calculations on the vertices. It's not done yet.
	/* glm::mat4 matrix = glm::scale(glm::mat4(1), scaling) * glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
	for (unsigned int i = 0; i < 8; i++) {
		vertices.at(i) *= matrix;
	}*/
}

bool BoundingBox3D::verticesWereGenerated() {
	return vertices.size() != 0;
}

glm::vec3 BoundingBox3D::getVertex(int id) {
	return vertices.at(id);
}

std::string BoundingBox3D::getVerticesAsString() {
	std::string s = "";
	for (unsigned int i = 0; i < vertices.size(); i++) {
		s += "Vertex " + std::to_string(i) + ": " + toStringWithATrailingZero(vertices.at(i).x) + ", "
			+ toStringWithATrailingZero(vertices.at(i).y) + ", " + toStringWithATrailingZero(vertices.at(i).z);
		if (i != vertices.size() - 1) {
			s += " - ";
		}
	}
	return s;
}

void BoundingBox3D::setWidth(float width) {
	this->width = width;
}

void BoundingBox3D::setHeight(float height) {
	this->height = height;
}

void BoundingBox3D::setDpeth(float depth) {
	this->depth = depth;
}

float BoundingBox3D::getWidth() {
	return width;
}

float BoundingBox3D::getHeight() {
	return height;
}

float BoundingBox3D::getDepth() {
	return depth;
}

void BoundingBox3D::setValues(float x, float y, float z, float width, float height, float depth) {
	position.x = x;
	position.y = y;
	position.z = z;
	this->width = width;
	this->height = height;
	this->depth = depth;
}
