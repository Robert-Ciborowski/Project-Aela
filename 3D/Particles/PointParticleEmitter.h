/*
* Class: Point Particle Emitter
* Author: Robert Ciborowski
* Date: 29/08/2017
* Description: A class used to emit particles from a point.
*/

#pragma once

#include "ParticleEmitter.h"
#include "../../Utilities/Rect/Rect.h"
#include "../Camera/Camera3D.h"

namespace Aela {
	class PointParticleEmitter : public ParticleEmitter {
		public:
			PointParticleEmitter(Clock* time) : ParticleEmitter(time) {

			}

			// These are the functions that are overwritten from the ParticleEmitter class.
			void setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount);
			void update();

			// These are getters and setters.
			Camera3D* getCamera();
			void setCamera(Camera3D* camera);

		private:
			// This class requires a pointer to the camera so that it can properly sort particles.
			Camera3D* camera;

			void setupParticlePositioning(size_t whichParticle, size_t numberOfParticles);
	};
}