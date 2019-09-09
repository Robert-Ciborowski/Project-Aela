#include "AudioClip.h"

using namespace Aela;

AudioClip::AudioClip(std::string src, char* data) : Resource(src) {
	this->data = data;
}

AudioClip::~AudioClip() {
	delete[] data;
}

const char* AudioClip::getData() {
	return data;
}

ALenum AudioClip::getFormat() {
	return format;
}

ALsizei AudioClip::getSize() {
	return size;
}

ALsizei AudioClip::getSampleRate() {
	return sampleRate;
}

float AudioClip::getDefaultVolume() {
	return defaultVolume;
}

float Aela::AudioClip::getCurrentVolume() {
	return currentVolume;
}

void AudioClip::setDefaultVolume(float defaultVolume) {
	this->defaultVolume = defaultVolume;
}

void AudioClip::setFormat(ALenum format) {
	this->format = format;
}

void AudioClip::setFormat(int channelCount, int bitsPerSample) {
	bool stereo = (channelCount > 1);

	switch (bitsPerSample) {
		case 16:
			if (stereo) {
				format = AL_FORMAT_STEREO16;
			} else {
				format = AL_FORMAT_MONO16;
			}
			break;
		case 8:
			if (stereo) {
				format = AL_FORMAT_STEREO8;
			} else {
				format = AL_FORMAT_MONO8;
			}
			break;
		default:
			format = -1;
			break;
	}
}

void AudioClip::setSize(ALsizei size) {
	this->size = size;
}

void AudioClip::setSampleRate(ALsizei sampleRate) {
	this->sampleRate = sampleRate;
}