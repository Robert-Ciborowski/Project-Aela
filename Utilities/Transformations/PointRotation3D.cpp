/*
* Class: Point Rotation
* Author: Robert Ciborowski
* Date: 04/08/2017
* Description: A class used to specify a rotation around a point.
*/

#include "PointRotation3D.h"

using namespace Aela;

glm::vec3* PointRotation3D::getRotation() {
	return &rotation;
}

void PointRotation3D::setRotation(glm::vec3* rotation) {
	this->rotation = *rotation;
}

glm::vec3* PointRotation3D::getPoint() {
	return &point;
}

void PointRotation3D::setPoint(glm::vec3* point) {
	this->point = *point;
}
