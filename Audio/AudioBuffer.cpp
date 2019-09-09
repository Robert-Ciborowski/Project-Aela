//
// Created by Robert on 03/09/2018.
//

#include "AudioBuffer.h"

AudioBuffer::AudioBuffer(ALuint buffer, ALuint source, AudioClip* clip) {
	this->buffer = buffer;
	this->source = source;
	this->clip = clip;
}

ALuint AudioBuffer::getBuffer() {
	return buffer;
}

ALuint AudioBuffer::getSource() {
	return source;
}

AudioClip* AudioBuffer::getClip() {
	return clip;
}

