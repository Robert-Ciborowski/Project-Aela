/*
* Class: Planar Particle Emitter
* Author: Robert Ciborowski
* Date: 16/07/2017
* Description: A class used to emit particles along a plane. The particles travel perpendicular to the plane.
*/

#include "PlanarParticleEmitter.h"

#include <glm/gtc/constants.hpp>

using namespace Aela;

void PlanarParticleEmitter::setupDimensions(Rect<GLfloat>* dimensions) {
	this->dimensions = *dimensions;
}

Aela::PlanarParticleEmitter::PlanarParticleEmitter(Clock* time) : ParticleEmitter(time) {

}

Aela::PlanarParticleEmitter::~PlanarParticleEmitter() {
	end();
}

void PlanarParticleEmitter::setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount) {
	ParticleEmitter::setupParticles(textures, particleWidth, particleHeight, amount);
}

void PlanarParticleEmitter::update() {
	// Note: the particle positioning is relative to the emitter. The rotation, position and scaling of the emitter is looked at by the renderer,
	// which renders the particles while taking these transformations into account.
	for (size_t i = 0; i < particles.size(); i++) {
		Particle* particle = &particles.at(i);
		if (particle->getProperty(Transformable3DProperty::Y_POSITION) >= particle->getDistance()) {
			setupParticlePositioning(i, particles.size());
		} else {
			particle->translate(0, time->getTimeBetweenFramesInNanos() * particle->getSpeed(), 0);
		}
	}
}

void PlanarParticleEmitter::setupParticlePositioning(size_t whichParticle, size_t numberOfParticles) {
	// This chooses the position of the particle, relative to the emitter.
	srand((unsigned int) (time->getCurrentTimeInNanos() + whichParticle));
	float particleZ = dimensions.getHeight() - (dimensions.getHeight() / numberOfParticles) * whichParticle + dimensions.getY();
	particles[whichParticle].setPosition((rand() % 100) / 100.0f * dimensions.getWidth() + dimensions.getX(),
		pathOffset * (rand() % 100) / 100.0f, particleZ);
	for (size_t i = 0; i < particles.size(); i++) {
		if (particles[i].getPosition()->z >= particles[whichParticle].getPosition()->z) {
			particles.insert(particles.begin() + i, particles[whichParticle]);
			particles.erase(particles.begin() + whichParticle + 1);
		}
	}
}
