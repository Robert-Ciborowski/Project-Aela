/*
* Class: Project Aela's 3D Key Frame
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on Transformables.
*/

#include "KeyFrame3D.h"

using namespace Aela;

KeyFrameType KeyFrame3D::getType() {
	return KeyFrameType::THREE_DIMENSIONAL;
}

void KeyFrame3D::setup() {
	if (object != nullptr) {
		originalPosition = *object->getPosition();
		originalRotation = *object->getRotation();
		originalScaling = *object->getScaling();
	}

	KeyFrame::setup();
}

void KeyFrame3D::setup(KeyFrame* previousKeyFrame) {
	KeyFrame3D* other = (KeyFrame3D*) previousKeyFrame;

	if (object != nullptr) {
		originalPosition = *other->getPosition();
		originalRotation = *other->getRotation();
		originalScaling = *other->getScaling();
	}

	KeyFrame::setup(previousKeyFrame);
}

void KeyFrame3D::setObject(Transformable3D* object) {
	this->object = object;
	position = *object->getPosition();
	rotation = *object->getRotation();
	scaling = *object->getScaling();
}

Transformable3D* KeyFrame3D::getObject() {
	return object;
}

void KeyFrame3D::setTranslation(glm::vec3* position) {
	this->position = *position;
}

void Aela::KeyFrame3D::setTranslation(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

glm::vec3* KeyFrame3D::getPosition() {
	return &position;
}

void KeyFrame3D::setRotation(glm::vec3* rotation) {
	this->rotation = *rotation;
}

void Aela::KeyFrame3D::setRotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

glm::vec3* KeyFrame3D::getRotation() {
	return &rotation;
}

void KeyFrame3D::setPointRotation(PointRotation3D* pointRotation) {
	this->pointRotation = *pointRotation;
}

PointRotation3D* KeyFrame3D::getPointRotation() {
	return &pointRotation;
}

PointRotation3D* Aela::KeyFrame3D::getOriginalPointRotation() {
	return &originalPointRotation;
}

void KeyFrame3D::setUseTranslation(bool use) {
	useTranslation = use;
}

void KeyFrame3D::setUseRotation(bool use) {
	useRotation = use;
}

void KeyFrame3D::setUsePointRotation(bool use) {
	usePointRotation = use;
}

void KeyFrame3D::setUseScaling(bool use) {
	useScaling = use;
}

bool KeyFrame3D::isUsingTranslation() {
	return useTranslation;
}

bool KeyFrame3D::isUsingRotation() {
	return useRotation;
}

bool KeyFrame3D::isUsingPointRotation() {
	return usePointRotation;
}

bool KeyFrame3D::isUsingScaling() {
	return useScaling;
}

void KeyFrame3D::setScaling(glm::vec3* scaling) {
	this->scaling = *scaling;
}

void Aela::KeyFrame3D::setScaling(float x, float y, float z) {
	scaling.x = x;
	scaling.y = y;
	scaling.z = z;
}

glm::vec3* KeyFrame3D::getScaling() {
	return &scaling;
}

glm::vec3* KeyFrame3D::getOriginalPosition() {
	return &originalPosition;
}

glm::vec3* KeyFrame3D::getOriginalRotation() {
	return &originalRotation;
}

glm::vec3* KeyFrame3D::getOriginalScaling() {
	return &originalScaling;
}
