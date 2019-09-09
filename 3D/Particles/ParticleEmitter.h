/*
* Class: Particle Emitter
* Author: Robert Ciborowski
* Date: 15/07/2017
* Description: An interface used to emit particles. Because particles can be emitted in so many ways,
*              the implementation for how they are emitted should be implemented in a sub class.
*/

#pragma once
#include <functional>
#include "Particle.h"
#include "../../Time/Clock.h"

namespace Aela {
	class ParticleEmitter : public Transformable3D {
		public:
			ParticleEmitter(Clock* time);

			virtual ~ParticleEmitter();

			// This sets up a particle. Basic implementation has been done on this function.
			virtual void setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight,
				unsigned int amount);

			// This is the update function, which has no actual implementation.
			virtual void update() = 0;

			// These are getters and setters.
			std::vector<Particle>* getParticles();
			void setStats(float baseSpeed, float baseDistance, float speedOffset, float distanceOffset, float pathOffset,
				long long lifeTime);
			float getBaseSpeed();
			void setBaseSpeed(float baseSpeed);
			float getBaseDistance();
			void setBaseDistance(float baseDistance);
			float getSpeedOffset();
			void setSpeedOffset(float speedOffset);
			float getDistanceOffset();
			void setDistanceOffset(float distanceOffset);
			float getPathOffset();
			void setPathOffset(float pathOffset);
			long long getLifeTime();
			void setLifeTime(long long lifeTime);
			void setActionOnEnd(std::function<void()> onEnd);

			// Returns a bool that states whether the emitter should be deleted.
			bool isExpired();

		protected:
			// These are the Aela classes that are used.
			std::vector<Particle> particles;
			Clock* time;

			// This defines properties of the particles. Speed = distance / nanosecond, lifetime = distance.
			float baseSpeed = 0.001f;
			float baseDistance = 1000;

			// These are offsets that allow some particles to be slightly different in their shape compared to other particles.
			// Note that pathOffset modifies where the path of the particle starts.
			float speedOffset = 0;
			float distanceOffset = 0;
			float pathOffset = 0;

			// These times are in nanoseconds.
			unsigned long long lifeTime = 1, startTime;

			std::function<void()> onEnd = NULL;

			void end();

			// This is meant to reset a particle's position once it has completed its life.
			virtual void setupParticlePositioning(size_t whichParticle, size_t numberOfParticles) = 0;
	};
}