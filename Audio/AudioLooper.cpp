#include "AudioLooper.h"
#include "../../Project Aela/Resource Management/ResourceManager.h"

Aela::AudioLooper::AudioLooper() {}

void Aela::AudioLooper::update() {
	std::vector<AudioClip*> clipsToErase;

	for (auto& pair : loopingClips) {
		auto& data = pair.second;
		if (!data.started && data.timeWhenToStart <= clock->getCurrentTimeInMillis()) {
			audioPlayer->playClip(pair.first);
			data.timeOfStart = clock->getCurrentTimeInMillis();
			data.started = true;
		}

		if (data.started && data.locationOfLoopEnd <= clock->getCurrentTimeInMillis() - data.timeOfStart) {
			audioPlayer->stopClip(pair.first);

			if (data.numberOfLoops != 0) {
				data.whichLoop++;

				if (data.whichLoop == data.numberOfLoops) {
					clipsToErase.push_back(pair.first);
					continue;
				}
			}

			audioPlayer->playClip(pair.first);
			data.timeOfStart = clock->getCurrentTimeInMillis();
		}
	}

	for (AudioClip* clip : clipsToErase) {
		loopingClips.erase(clip);
	}

	std::vector<AudioStream*> streamsToErase;

	for (auto& pair : loopingStreams) {
		auto& data = pair.second;
		if (!data.started && data.timeWhenToStart <= clock->getCurrentTimeInMillis()) {
			audioPlayer->playStream(pair.first);
			data.timeOfStart = clock->getCurrentTimeInMillis();
			data.started = true;
		}

		if (data.started && data.locationOfLoopEnd <= clock->getCurrentTimeInMillis() - data.timeOfStart) {
			audioPlayer->stopStream(pair.first);

			if (data.numberOfLoops != 0) {
				data.whichLoop++;

				if (data.whichLoop == data.numberOfLoops) {
					streamsToErase.push_back(pair.first);
					continue;
				}
			}

			audioPlayer->playStream(pair.first);
			data.timeOfStart = clock->getCurrentTimeInMillis();
		}
	}

	for (AudioStream* stream : streamsToErase) {
		loopingStreams.erase(stream);
	}
}

void Aela::AudioLooper::loopAudioClipUsingSrc(std::string src, unsigned numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioClip* clip;
	if (resourceManager->obtain<AudioClip>(src, clip)) {
		loopAudioClip(clip, numberOfLoops, locationOfLoopEnd, waitTimeBeforeStart);
	}
}

void Aela::AudioLooper::loopAudioStreamUsingSrc(std::string src, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioStream* stream;

	if (resourceManager->obtain<AudioStream>(src, stream)) {
		loopAudioStream(stream, numberOfLoops, locationOfLoopEnd, waitTimeBeforeStart);
	}
}

void Aela::AudioLooper::loopAudioClip(Aela::AudioClip* clip, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioLoopData data;
	data.whichLoop = 0;
	data.numberOfLoops = numberOfLoops;
	data.started = false;
	data.timeWhenToStart = waitTimeBeforeStart + clock->getCurrentTimeInMillis();
	data.locationOfLoopEnd = locationOfLoopEnd;
	loopingClips[clip] = data;
}

void Aela::AudioLooper::loopAudioStream(AudioStream* stream, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioLoopData data;
	data.whichLoop = 0;
	data.numberOfLoops = numberOfLoops;
	data.started = false;
	data.timeWhenToStart = waitTimeBeforeStart + clock->getCurrentTimeInMillis();
	data.locationOfLoopEnd = locationOfLoopEnd;
	loopingStreams[stream] = data;
	std::cout << "Now looping stream " << stream->getSrc();
}

void Aela::AudioLooper::loopAudioClipInfinitelyUsingSrc(std::string src, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioClip* clip;
	if (resourceManager->obtain<AudioClip>(src, clip)) {
		loopAudioClipInfinitely(clip, locationOfLoopEnd, waitTimeBeforeStart);
	}
}

void Aela::AudioLooper::loopAudioStreamInfinitelyUsingSrc(std::string src, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioStream* stream;
	if (resourceManager->obtain<AudioStream>(src, stream)) {
		loopAudioStreamInfinitely(stream, locationOfLoopEnd, waitTimeBeforeStart);
	}
}

void Aela::AudioLooper::loopAudioClipInfinitely(Aela::AudioClip* clip, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	// Note: a number of loops of zero means that the audio loops infinitely.
	AudioLoopData data;
	data.whichLoop = 0;
	data.numberOfLoops = 0;
	data.started = false;
	data.timeWhenToStart = waitTimeBeforeStart + clock->getCurrentTimeInMillis();
	data.locationOfLoopEnd = locationOfLoopEnd;
	loopingClips[clip] = data;
}

void Aela::AudioLooper::loopAudioStreamInfinitely(AudioStream* stream, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioLoopData data;
	data.whichLoop = 0;
	data.numberOfLoops = 0;
	data.started = false;
	data.timeWhenToStart = waitTimeBeforeStart + clock->getCurrentTimeInMillis();
	data.locationOfLoopEnd = locationOfLoopEnd;
	loopingStreams[stream] = data;
}

bool Aela::AudioLooper::stopClipLoopUsingSrc(std::string src) {
	AudioClip* clip;

	if (resourceManager->obtain<AudioClip>(src, clip)) {
		stopClipLoop(clip);
		return true;
	}

	return false;
}

bool Aela::AudioLooper::stopStreamLoopUsingSrc(std::string src) {
	AudioStream* stream;

	if (resourceManager->obtain<AudioStream>(src, stream)) {
		stopStreamLoop(stream);
		return true;
	}

	return false;
}

bool Aela::AudioLooper::stopClipLoop(Aela::AudioClip* clip) {
	auto pair = loopingClips.find(clip);

	if (pair != loopingClips.end()) {
		audioPlayer->stopClip(clip);
		loopingClips.erase(pair);
		return true;
	}

	return false;
}

bool Aela::AudioLooper::stopStreamLoop(AudioStream* stream) {
	auto pair = loopingStreams.find(stream);

	if (pair != loopingStreams.end()) {
		audioPlayer->stopStream(stream);
		loopingStreams.erase(pair);
		return true;
	}

	return false;
}

void Aela::AudioLooper::stopAllLoops() {
	for (auto pair : loopingClips) {
		audioPlayer->stopClip(pair.first);
	}

	for (auto pair : loopingStreams) {
		audioPlayer->stopStream(pair.first);
	}

	loopingClips.clear();
	loopingStreams.clear();
}

bool AudioLooper::isLoopingAnyAudio() {
	return !loopingClips.empty() || loopingStreams.empty();
}

size_t AudioLooper::getNumberOfCurrentLoops() {
	return loopingStreams.size();
}

AudioClip* AudioLooper::getFirstLoopingClip() {
	if (loopingClips.empty()) {
		return nullptr;
	}

	return loopingClips.begin()->first;
}

AudioLoopData* AudioLooper::getFirstLoopDataClip() {
	if (loopingClips.empty()) {
		return nullptr;
	}

	return &loopingClips.begin()->second;
}

AudioStream* Aela::AudioLooper::getFirstLoopingStream() {
	if (loopingStreams.empty()) {
		return nullptr;
	}

	return loopingStreams.begin()->first;
}

AudioLoopData* Aela::AudioLooper::getFirstLoopDataStream() {
	if (loopingStreams.empty()) {
		return nullptr;
	}

	return &loopingStreams.begin()->second;
}

void AudioLooper::setAudioPlayer(AudioPlayer* audioPlayer) {
	this->audioPlayer = audioPlayer;
}

void AudioLooper::setResourceManager(ResourceManager* resourceManager) {
	this->resourceManager = resourceManager;
}

void AudioLooper::setClock(Clock* clock) {
	this->clock = clock;
}
