/*
* Class: Particle Emitter
* Author: Robert Ciborowski
* Date: 15/07/2017
* Description: An interface used to emit particles. Because particles can be emitted in so many ways,
*              the implementation for how they are emitted should be implemented in a sub class.
*/

#include "ParticleEmitter.h"

#include <algorithm>
#include <functional>

using namespace Aela;

Aela::ParticleEmitter::ParticleEmitter(Clock* time) {
	this->time = time;
	startTime = time->getCurrentTimeInNanos();
}

Aela::ParticleEmitter::~ParticleEmitter() {
	end();
}

void ParticleEmitter::setupParticles(std::vector<GLTexture*>* textures, float particleWidthScaling, float particleHeightScaling, unsigned int amount) {
	for (unsigned int i = 0; i < amount; i++) {
		Particle particle;
		particle.setTexture(textures->at(rand() % textures->size()));
		particle.setProperty(Transformable3DProperty::X_SCALING, particleWidthScaling);
		particle.setProperty(Transformable3DProperty::Y_SCALING, particleHeightScaling);
		particle.setSpeed(baseSpeed + (speedOffset * (rand() % 100) / 100));
		particle.setDistance((unsigned int) (baseDistance + (distanceOffset * (rand() % 100) / 100)));
		particles.push_back(particle);
		setupParticlePositioning(i, amount);
	}
}

std::vector<Particle>* ParticleEmitter::getParticles() {
	return &particles;
}

void ParticleEmitter::setStats(float baseSpeed, float baseDistance, float speedOffset, float distanceOffset,
	float pathOffset, long long lifeTime) {
	this->baseSpeed = baseSpeed;
	this->baseDistance = baseDistance;
	this->speedOffset = speedOffset;
	this->distanceOffset = distanceOffset;
	this->pathOffset = pathOffset;
	this->lifeTime = lifeTime;
}

float ParticleEmitter::getBaseSpeed() {
	return baseSpeed;
}

void ParticleEmitter::setBaseSpeed(float baseSpeed) {
	this->baseSpeed = baseSpeed;
}

float ParticleEmitter::getBaseDistance() {
	return baseDistance;
}

void ParticleEmitter::setBaseDistance(float baseDistance) {
	this->baseDistance = baseDistance;
}

float ParticleEmitter::getSpeedOffset() {
	return speedOffset;
}

void ParticleEmitter::setSpeedOffset(float speedOffset) {
	this->speedOffset = speedOffset;
}

float ParticleEmitter::getDistanceOffset() {
	return distanceOffset;
}

void ParticleEmitter::setDistanceOffset(float distanceOffset) {
	this->distanceOffset = distanceOffset;
}

float ParticleEmitter::getPathOffset() {
	return pathOffset;
}

void ParticleEmitter::setPathOffset(float pathOffset) {
	this->pathOffset = pathOffset;
}

long long Aela::ParticleEmitter::getLifeTime() {
	return lifeTime;
}

void Aela::ParticleEmitter::setLifeTime(long long lifeTime) {
	this->lifeTime = lifeTime;
}

bool Aela::ParticleEmitter::isExpired() {
	return time->getCurrentTimeInNanos() >= startTime + lifeTime;
}

// The implementation of this function is to be done by the sub class. The function is meant to reset the particle's position once it has completed
// its life. This creates the effect of spawning in a new particle, even though its actually just resetting the particle in the pool.
void ParticleEmitter::setupParticlePositioning(size_t whichParticle, size_t numberOfParticles) {
}

void ParticleEmitter::setActionOnEnd(std::function<void()> onEnd) {
	this->onEnd = onEnd;
}

void ParticleEmitter::end() {
	if (onEnd != NULL) {
		onEnd();
	}
}