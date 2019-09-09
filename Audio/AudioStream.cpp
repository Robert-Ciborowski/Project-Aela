#include "AudioStream.h"
#include "../Utilities/alut.h"

#define QUEUE_SIZE 12
#define BITS_PER_SAMPLE 16
#define SAMPLES_PER_DATA_CHUNK 4096
#define BYTES_TO_SAMPLE_DIVISOR 4

Aela::AudioStream::AudioStream(std::string src) : Resource(src) {
}

Aela::AudioStream::~AudioStream() {
	alDoWithErrorCheck_noret(alDeleteSources(1, &source));
	deleteSourceAndBuffers();
}

void Aela::AudioStream::start(int generalVolume) {
	if (playing) {
		stop();
	}

	timeOfLastUpdate = clock.getCurrentTimeInNanos();
	paused = false;
	playing = true;

	// if (source == NULL) {
		alDoWithErrorCheck_void(alGenSources(1, &source));
		alDoWithErrorCheck_void(alSourcei(source, AL_LOOPING, AL_FALSE));
		alDoWithErrorCheck_void(alSourcef(source, AL_PITCH, 1.0F));
		alDoWithErrorCheck_void(alSourcef(source, AL_GAIN, 1.0F));
		alDoWithErrorCheck_void(alSource3f(source, AL_POSITION, 0, 0, 0));
		alDoWithErrorCheck_void(alSource3f(source, AL_VELOCITY, 0, 0, 0));
	// }

	setupQueues();

	// alDoWithErrorCheck_void(alSourcei(source, AL_BUFFER, buffers[currentBuffer]));
	alDoWithErrorCheck_noret(alSourcePlay(source));
	alDoWithErrorCheck_noret(alGetSourcei(source, AL_SOURCE_STATE, &sourceState));
	alSourcef(source, AL_GAIN, defaultVolume * (float) generalVolume / 100);
}

void Aela::AudioStream::setupQueues() {
	if (!buffers.empty()) {
		deleteSourceAndBuffers();
	}

	for (int i = 0; i < QUEUE_SIZE; i++) {
		ALuint buffer;
		alDoWithErrorCheck_void(alGenBuffers(1, &buffer));
		buffers.push_back(buffer);
		loadDataIntoBuffer(buffer);
	}

	alDoWithErrorCheck_void(alSourceQueueBuffers(source, QUEUE_SIZE, buffers.data()));
}

void Aela::AudioStream::loadDataIntoBuffer(ALuint buffer) {}

void Aela::AudioStream::currentBufferIsFinished() {
	alDoWithErrorCheck_void(alSourceUnqueueBuffers(source, 1, &buffers[currentBuffer]));
	loadDataIntoBuffer(buffers[currentBuffer]);

	if (!playing) {
		return;
	}

	alDoWithErrorCheck_void(alSourceQueueBuffers(source, 1, &buffers[currentBuffer]));
	currentBuffer = (currentBuffer + 1) % QUEUE_SIZE;
}

void Aela::AudioStream::deleteSourceAndBuffers() {
	if (buffers.empty()) {
		return;
	}

	alSourceStop(source);
	alDoWithErrorCheck_noret(alDeleteSources(1, &source));

	// Note: the OpenAL error that is received here only occurs once the track
	// is finished playing.

	// alDoWithErrorCheck_void(alSourceUnqueueBuffers(source, QUEUE_SIZE, &buffers[0]));
	alDoWithErrorCheck_void(alDeleteBuffers(QUEUE_SIZE, &buffers[0]));
	buffers.resize(0);
}

void Aela::AudioStream::update() {
	if (!playing) {
		return;
	}

	if (paused) {
		timeOfLastUpdate = clock.getCurrentTimeInNanos();
		return;
	}

	unsigned long long currentTime = clock.getCurrentTimeInNanos();
	progress += currentTime - timeOfLastUpdate;
	timeOfLastUpdate = currentTime;

	alDoWithErrorCheck_noret(alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed));

	if (buffersProcessed == 0) {
		return;
	}

	while (buffersProcessed != 0) {
		ALuint current = buffers[currentBuffer];
		currentBufferIsFinished();

		if (buffers.empty()) {
			playing = false;
		}

		buffersProcessed--;
	}
}

void Aela::AudioStream::pause() {
	paused = true;
}

void Aela::AudioStream::unpause() {
	paused = false;
}

void Aela::AudioStream::stop() {
	playing = false;
	progress = 0;
	paused = false;
	deleteSourceAndBuffers();
	samplesLoaded = 0;
}

void Aela::AudioStream::seekToTime(unsigned long long timeInNanos) {
	/*if (timeInNanos > lengthInNanos) {
		timeInNanos = lengthInNanos;
	}

	progress = timeInNanos;*/

	AelaErrorHandling::consoleWindowError("Audio Stream", "seekToTime is currently unsupported");
}

bool Aela::AudioStream::isPlaying() {
	return playing;
}

ALenum Aela::AudioStream::getFormat() {
	return format;
}

ALsizei Aela::AudioStream::getSampleRate() {
	return sampleRate;
}

float Aela::AudioStream::getDefaultVolume() {
	return defaultVolume;
}

float Aela::AudioStream::getVolume() {
	return volume;
}

void Aela::AudioStream::setFormat(ALenum format) {
	this->format = format;
}

void Aela::AudioStream::setFormat(int channelCount, int bitsPerSample) {
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

void Aela::AudioStream::setSampleRate(ALsizei sampleRate) {
	this->sampleRate = sampleRate;
}

void Aela::AudioStream::setDefaultVolume(float defaultVolume) {
	this->defaultVolume = defaultVolume;
}