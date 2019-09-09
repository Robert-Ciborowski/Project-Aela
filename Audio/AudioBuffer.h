/*
* Class: Audio Buffer
* Author: Robert Ciborowski
* Date: 03/09/2018
* Description: Represents an audio buffer. I made this as a temporary thing
               while audio pooling gets implemented so that playing audio
               doesn't block the main thread. However, it might be useful
               even after pools are implemented. Feel free to add to this.
               I made it a class (and not a struct) in case anyone wants to
               add extra methods to this.
*/

#pragma once


#include <al.h>
#include "AudioClip.h"

// Shouldn't this actually be in the Aela namespace?
using namespace Aela;

class AudioBuffer {
	public:
		AudioBuffer(ALuint buffer, ALuint source, AudioClip* clip);

		ALuint getBuffer();
		ALuint getSource();
		AudioClip* getClip();

	private:
		ALuint buffer, source;
		AudioClip* clip;
};

