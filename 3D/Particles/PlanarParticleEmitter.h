/*
* Class: Planar Particle Emitter
* Author: Robert Ciborowski
* Date: 16/07/2017
* Description: A class used to emit particles along a plane. The particles travel perpendicular to the plane.
*/

#pragma once

#include "ParticleEmitter.h"
#include "../../Utilities/Rect/Rect.h"
#include "../Camera/Camera3D.h"

namespace Aela {
	class PlanarParticleEmitter : public ParticleEmitter {
		public:
			PlanarParticleEmitter(Clock* time);
			virtual ~PlanarParticleEmitter();

			// These are the functions that are overwritten from the ParticleEmitter class.
			virtual void setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount);
			virtual void update();

			void setupDimensions(Rect<GLfloat>* dimensions);

		private:
			// This stores the dimensions of the plane.
			Rect<GLfloat> dimensions;

			virtual void setupParticlePositioning(size_t whichParticle, size_t numberOfParticles);
	};
}