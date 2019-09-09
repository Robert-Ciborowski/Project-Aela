/*
* Class: LightEntity
* Author: Robert Ciborowski
* Date: 07/04/2017
* Description: A class used to store the properties of a light.
*/

#include "LightEntity.h"
#include <iostream>

using namespace Aela;

Aela::LightEntity::LightEntity() {
	useDefaultValues();
}

Aela::LightEntity::LightEntity(glm::vec3 position, glm::vec3 rotation, ColourRGB* colour, float power) {
	this->position = position;
	this->rotation = rotation;
	this->colour = *colour;
	this->power = power;
}

Aela::LightEntity::~LightEntity() {
	if (setUpForShadows) {
		glDeleteTextures(1, &shadowMapTexture);
		glDeleteFramebuffers(1, &shadowMapBuffer);
	}
}

void LightEntity::useDefaultValues() {
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scaling = glm::vec3(1, 1, 1);
	colour.set(1.0, 1.0, 1.0);
	power = 1.0;
}

void LightEntity::setupForNewShadowMap() {
	shadowMapBuffer = 0;
	shadowMapTexture = 0;
	setUpForShadows = false;
}

void LightEntity::setColour(ColourRGB* colour) {
	this->colour = *colour;
}

void LightEntity::setColour(float r, float g, float b) {
	colour.set(r, g, b);
}

void LightEntity::setPower(float power) {
	this->power = power;
}

ColourRGB* LightEntity::getColour() {
	return &colour;
}

float LightEntity::getPower() {
	return power;
}

GLuint* LightEntity::getShadowMapTexture() {
	return &shadowMapTexture;
}

void LightEntity::setShadowMapTexture(GLuint* shadowMapTexture) {
	this->shadowMapTexture = *shadowMapTexture;
}

GLuint* LightEntity::getShadowMapBuffer() {
	return &shadowMapBuffer;
}

void LightEntity::setShadowMapBuffer(GLuint* shadowMapBuffer) {
	this->shadowMapBuffer = *shadowMapBuffer;
}

EntityType LightEntity::getEntityType() {
	return EntityType::LIGHT;
}

void LightEntity::hasBeenSetUpForShadows() {
	setUpForShadows = true;
}

bool LightEntity::isSetUpForShadows() {
	return setUpForShadows;
}
