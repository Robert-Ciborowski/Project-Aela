/*
* Class: Project Aela's 2D Key Frame
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used by Aela's animator to perform transformations on 2D objects.
*/

#include "KeyFrame2D.h"
#include <iostream>

using namespace Aela;

void KeyFrame2D::setup() {
	if (object != nullptr) {
		originalTint = *object->getTint();
		originalDimensions = *object->getDimensions();
	}

	if (useCurvature) {
		setupCurvatureVariables();
	}

	KeyFrame::setup();
}

void KeyFrame2D::setup(KeyFrame* previousKeyFrame) {
	KeyFrame2D* other = (KeyFrame2D*) previousKeyFrame;
	if (object != nullptr) {
		originalTint = *other->getTint();
		originalDimensions = *other->getDimensions();
	}

	if (useCurvature) {
		setupCurvatureVariables();
	}

	KeyFrame::setup(previousKeyFrame);
}

void KeyFrame2D::end() {
	KeyFrame::end();

	if (object != nullptr) {
		if (useTint) {
			object->setTint(&tint);
		}

		if (useDimensions) {
			object->setDimensions(&dimensions);
		}
	}
}

ColourRGBA KeyFrame2D::getTintAtTime(long long currentTime, long long endTime) {
	float timeRatio = (float) currentTime / endTime;
	ColourRGBA newTint((float) (tint.getR() - originalTint.getR()) * timeRatio + originalTint.getR(),
		(float) (tint.getG() - originalTint.getG()) * timeRatio + originalTint.getG(),
		(float) (tint.getB() - originalTint.getB()) * timeRatio + originalTint.getB(),
		(float) (tint.getA() - originalTint.getA()) * timeRatio + originalTint.getA());
	return newTint;
}

Rect<int> KeyFrame2D::getDimensionsAtTime(long long currentTime, long long endTime) {
	float timeRatio = (float) currentTime / endTime;
	int width = (int) ((dimensions.getWidth() - originalDimensions.getWidth()) * timeRatio + originalDimensions.getWidth());
	int height = (int) ((dimensions.getHeight() - originalDimensions.getHeight()) * timeRatio + originalDimensions.getHeight());
	int x, y;

	if (useCurvature) {
		glm::vec2 offset = getCurvatureOffset(timeRatio);
		x = (int) (originalDimensions.getX() + offset.x);
		y = (int) (originalDimensions.getY() + offset.y);
	} else {
		x = (int) ((dimensions.getX() - originalDimensions.getX()) * timeRatio + originalDimensions.getX());
		y = (int) ((dimensions.getY() - originalDimensions.getY()) * timeRatio + originalDimensions.getY());
	}

    Rect<int> returnValue(x, y, width, height);
	return returnValue;
}

Rect<int> KeyFrame2D::getDimensionsAtTime2(long long currentTime, long long endTime) {
	if (currentTime >= endTime) {
		return dimensions;
	}

	float timeRatio = (float) currentTime / endTime;
	int width = (int) ((dimensions.getWidth() - originalDimensions.getWidth()) * timeRatio + originalDimensions.getWidth());
	int height = (int) ((dimensions.getHeight() - originalDimensions.getHeight()) * timeRatio + originalDimensions.getHeight());
	int x, y;

	if (useCurvature) {
		glm::vec2 offset = getCurvatureOffset(timeRatio);
		x = (int) (originalDimensions.getX() + offset.x);
		y = (int) (originalDimensions.getY() + offset.y);
	} else {
		x = (int) ((dimensions.getX() - originalDimensions.getX()) * timeRatio + originalDimensions.getX());
		y = (int) ((dimensions.getY() - originalDimensions.getY()) * timeRatio + originalDimensions.getY());
	}

    Rect<int> returnValue(x, y, width, height);
    return returnValue;
}

KeyFrameType KeyFrame2D::getType() {
	return KeyFrameType::TWO_DIMENSIONAL;
}

void KeyFrame2D::setObject(std::shared_ptr<Transformable2D> object) {
	this->object = object;
}

std::shared_ptr<Transformable2D> KeyFrame2D::getObject() {
	return object;
}

void KeyFrame2D::setTint(ColourRGBA* tint) {
	this->tint = *tint;
	useTint = true;
}

ColourRGBA* KeyFrame2D::getTint() {
	return &tint;
}

void Aela::KeyFrame2D::setOriginalTint(ColourRGBA* originalTint) {
	this->originalTint = *originalTint;
}

ColourRGBA* KeyFrame2D::getOriginalTint() {
	return &originalTint;
}

void KeyFrame2D::setDimensions(Rect<int>* dimensions) {
	this->dimensions = *dimensions;
	useDimensions = true;
}

Rect<int>* KeyFrame2D::getDimensions() {
	return &dimensions;
}

Rect<int>* KeyFrame2D::getOriginalDimensions() {
	return &originalDimensions;
}

bool Aela::KeyFrame2D::isUsingDimensions() {
	return useDimensions;
}

bool KeyFrame2D::isUsingTint() {
	return useTint;
}

void KeyFrame2D::setCurvatureToMovement(float curvatureMultiplier) {
	this->curvatureMultiplier = curvatureMultiplier;
	useCurvature = true;
}

bool KeyFrame2D::isUsingCurvature() {
	return useCurvature;
}

glm::vec2 KeyFrame2D::getCurvatureOffset(float ratioOfTimeTaken) {
	float linearDistance = ratioOfTimeTaken * d;
	float curveResult = curvatureFunction(linearDistance);
	float x = linearDistance * cosA - curveResult * sinA;
	float y = linearDistance * sinA + curveResult * cosA;
	return glm::vec2(x, y);
}

void KeyFrame2D::setupCurvatureVariables() {
	// This precomputes some values.
	float x2 = (float) dimensions.getX(), x1 = (float) originalDimensions.getX();
	float y2 = (float) dimensions.getY(), y1 = (float) originalDimensions.getY();
	float yDifference = y2 - y1, xDifference = x2 - x1;
	float angle = atan2(yDifference, xDifference);
	sinA = sin(angle);
	cosA = cos(angle);
	mSquared = curvatureMultiplier * curvatureMultiplier;
	d = sqrt(yDifference * yDifference + xDifference * xDifference);
	r = d / 2;
	rSquared = r * r;

	if (curvatureMultiplier >= 0) {
		curvatureSign = 1;
	} else {
		curvatureSign = -1;
	}
}

float KeyFrame2D::curvatureFunction(float linearDistanceTravelled) {
	float x = linearDistanceTravelled - r;
	return curvatureSign * sqrt(mSquared * (rSquared - x * x));
}
