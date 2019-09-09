/*
* Class: AudioListener
* Author: Robert Ciborowski
* Date: 29/06/2017
* Description: Represents the entity that is perceiving sound.
*/

#pragma once
#include <al.h>
#include <alc.h>
#include <glm/glm.hpp>

namespace Aela {
	class AudioListener {
		public:
			AudioListener();
			~AudioListener();

			bool setPosition(ALfloat x, ALfloat y, ALfloat z);
			bool setVelocity(ALfloat x, ALfloat y, ALfloat z);
			bool setOrientation(ALfloat x, ALfloat y, ALfloat z);

		private:
			ALfloat orientation[6];
	};
}
