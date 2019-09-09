/*
* Class: Particle
* Author: Robert Ciborowski
* Date: 15/07/2017
* Description: A class used to store properties of a particle.
*/

#include "Particle.h"

using namespace Aela;

float Particle::getSpeed() {
	return baseSpeed;
}

void Particle::setSpeed(float baseSpeed) {
	this->baseSpeed = baseSpeed;
}

unsigned int Particle::getDistance() {
	return baseDistance;
}

void Particle::setDistance(unsigned int baseDistance) {
	this->baseDistance = baseDistance;
}
