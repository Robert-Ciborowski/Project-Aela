#include "AudioPlayer.h"
#include "../Error Handler/ErrorHandling.h"
#include "../Utilities/alut.h"
#include <iostream>

using namespace Aela;

AudioPlayer::AudioPlayer() {
	clipMutex = new std::mutex();
	streamMutex = new std::mutex();
	closeThreadMutex = new std::mutex();
}

AudioPlayer::~AudioPlayer() {
	die();
}

void AudioPlayer::clearErrors() {
	alGetError();
}

bool AudioPlayer::init() {
	audioDevice = alcOpenDevice(nullptr);
	if (!audioDevice) {
		return false;
	}

	context = alcCreateContext(audioDevice, nullptr);
	alcMakeContextCurrent(context);

	if (!context) {
		return false;
	}

	backgroundThread = new std::thread(&AudioPlayer::update, this);

	return true;
}

void AudioPlayer::update() {
	ALint sourceState;

	// This is temporary stuff until the pools are added. I just want to listen
	// to audio while doing other things.
	while (true) {
		{
			std::lock_guard<std::mutex> guard(*closeThreadMutex);
			if (closeThread) {
				break;
			}
		}

		for (size_t i = 0; i < playingBuffers.size(); i++) {
			{
				std::lock_guard<std::mutex> guard(*clipMutex);
				AudioBuffer& audioBuffer = playingBuffers[i];

				alDoWithErrorCheck_noret(alGetSourcei(audioBuffer.getSource(), AL_SOURCE_STATE, &sourceState));

				if (sourceState != AL_PLAYING) {
					ALuint source = audioBuffer.getSource();
					ALuint buffer = audioBuffer.getBuffer();
					alDoWithErrorCheck_noret(alDeleteSources(1, &source));
					alDoWithErrorCheck_noret(alDeleteBuffers(1, &buffer));

					playingBuffers.erase(playingBuffers.begin() + i);
					i--;
				}
			}
		}

		for (size_t i = 0; i < playingStreams.size(); i++) {
			{
				std::lock_guard<std::mutex> guard(*streamMutex);
				AudioStream* stream = playingStreams[i];
				stream->update();

				if (!stream->playing) {
					stream->stop();
					playingStreams.erase(playingStreams.begin() + i);
					i--;
				}
			}
		}
	}
}

void AudioPlayer::die() {
	if (context != nullptr) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
	}

	if (audioDevice != nullptr) {
		alcCloseDevice(audioDevice);
	}

	// I think you might want to do this lol
	// - Robert
	{
		std::lock_guard<std::mutex> guard(*closeThreadMutex);
		closeThread = true;
	}

	backgroundThread->join();

	delete clipMutex;
	delete streamMutex;
	delete closeThreadMutex;
}

void AudioPlayer::playClip(AudioClip* clip) {
	if (clip == nullptr) {
		AelaErrorHandling::consoleWindowError("AudioPlayer", "AudioClip was null.");
		return;
	}

	// TEMPORARY audio playing code
	// TODO refactor to use a source pool, and a buffer pool
	// TODO determine cause of corruption noise

	clearErrors();

	ALuint buffer, source;
	ALint sourceState;
	alDoWithErrorCheck_void(alGenBuffers(1, &buffer));
	alDoWithErrorCheck_void(alBufferData(buffer, clip->getFormat(), clip->getData(), clip->getSize(), clip->getSampleRate()));

	alDoWithErrorCheck_void(alGenSources(1, &source));

	alDoWithErrorCheck_void(alSourcei(source, AL_BUFFER, buffer));
	alDoWithErrorCheck_void(alSourcei(source, AL_LOOPING, AL_FALSE));

	alDoWithErrorCheck_void(alSourcef(source, AL_PITCH, 1.0F));
	alDoWithErrorCheck_void(alSourcef(source, AL_GAIN, 1.0F));

	alDoWithErrorCheck_void(alSource3f(source, AL_POSITION, 0, 0, 0));
	alDoWithErrorCheck_void(alSource3f(source, AL_VELOCITY, 0, 0, 0));
	
	alDoWithErrorCheck_noret(alSourcePlay(source));
	alDoWithErrorCheck_noret(alGetSourcei(source, AL_SOURCE_STATE, &sourceState));

	alSourcef(source, AL_GAIN, clip->getDefaultVolume() * (float) generalVolume / 100);

	AudioBuffer audioBuffer(buffer, source, clip);
	{
		std::lock_guard<std::mutex> guard(*clipMutex);
		playingBuffers.push_back(audioBuffer);
	}
}

void AudioPlayer::stopClip(AudioClip* clip) {
	{
		std::lock_guard<std::mutex> guard(*clipMutex);

		// I don't think that playingClips was ever finished, so more stuff
		// will have to be done inside of the if statement of this loop.
		/*for (size_t i = 0; i < playingClips.size(); i++) {
			if (playingClips[i] == clip) {
			}
		}*/
		// Nevermind, we don't have to do that.

		for (size_t i = 0; i < playingBuffers.size(); i++) {
			if (playingBuffers[i].getClip() == clip) {
				alSourceStop(playingBuffers[i].getSource());
			}
		}
	}
}

bool AudioPlayer::isPlaying(AudioClip* clip) {
	// return std::find(playingClips.begin(), playingClips.end(), clip) != playingClips.end();

	for (size_t i = 0; i < playingBuffers.size(); i++) {
		if (playingBuffers[i].getClip() == clip) {
			return true;
		}
	}

	return false;
}

bool AudioPlayer::changeVolumeOfPlayingClip(AudioClip* clip, float volume) {
	// This kind of linear search sucks. Maybe we should instead use some sort of a map that uses AudioClip*.
	for (size_t i = 0; i < playingBuffers.size(); i++) {
		AudioBuffer& buffer = playingBuffers[i];
		if (buffer.getClip() == clip) {
			alSourcef(buffer.getSource(), AL_GAIN, volume * ( float) generalVolume / 100);
			buffer.getClip()->currentVolume = volume;
			return true;
		}
	}

	return false;
}

void Aela::AudioPlayer::setGeneralVolume(int generalVolume) {
	if (generalVolume > 100) {
		generalVolume = 100;
	} else if (generalVolume < 0) {
		generalVolume = 0;
	}

	this->generalVolume = generalVolume;

	for (auto& buffer : playingBuffers) {
		alSourcef(buffer.getSource(), AL_GAIN, buffer.getClip()->getCurrentVolume() * (float) generalVolume / 100);
	}

	for (auto& stream : playingStreams) {
		alSourcef(stream->source, AL_GAIN, stream->volume * (float) generalVolume / 100);
	}
}

int Aela::AudioPlayer::getGeneralVolume() {
	return generalVolume;
}

void Aela::AudioPlayer::playStream(AudioStream* stream) {
	if (stream == nullptr) {
		AelaErrorHandling::consoleWindowError("AudioPlayer", "AudioStream was null.");
		return;
	}

	clearErrors();
	stream->start(generalVolume);
	
	{
		std::lock_guard<std::mutex> guard(*streamMutex);
		playingStreams.push_back(stream);
	}
}

void Aela::AudioPlayer::stopStream(AudioStream* stream) {
	if (stream == nullptr) {
		AelaErrorHandling::consoleWindowError("AudioPlayer", "AudioStream was null.");
		return;
	}

	{
		std::lock_guard<std::mutex> guard(*streamMutex);
		auto iter = std::find(playingStreams.begin(), playingStreams.end(), stream);

		if (iter != playingStreams.end()) {
			playingStreams.at(iter - playingStreams.begin())->stop();
		} else {
			return;
		}
	}

	clearErrors();
	// stream->stop();
}

bool Aela::AudioPlayer::isPlaying(AudioStream* stream) {
	auto iter = std::find(playingStreams.begin(), playingStreams.end(), stream);
	return iter != playingStreams.end();
}

bool Aela::AudioPlayer::changeVolumeOfPlayingStream(AudioStream* stream, float volume) {
	if (stream == nullptr) {
		AelaErrorHandling::consoleWindowError("AudioPlayer", "AudioStream was null.");
		return false;
	}

	auto iter = std::find(playingStreams.begin(), playingStreams.end(), stream);

	if (iter != playingStreams.end()) {
		auto& stream = playingStreams.at(iter - playingStreams.begin());
		stream->volume = volume;
		alSourcef(stream->source, AL_GAIN, volume * ( float) generalVolume / 100);
		return true;
	}

	return false;
}
