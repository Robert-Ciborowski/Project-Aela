#include "SimpleAudioPlayer.h"
#include "../Error Handler/ErrorHandling.h"

Aela::SimpleAudioPlayer::SimpleAudioPlayer() {}

bool Aela::SimpleAudioPlayer::addAudioAsClip(std::string tag, std::string src) {
	Aela::AudioClip* clip;

	if (resourceManager->obtain<AudioClip>("res/audio/clips/" + src, clip)) {
		clips[tag] = clip;
		return true;
	}

	AelaErrorHandling::consoleWindowError("Simple Audio Player", "Did not find stream res/audio/clips/" + src);
	return false;
}

bool Aela::SimpleAudioPlayer::addAudioAsStream(std::string tag, std::string src) {
	// Once streams are added, we'll use streams. For now, we're just loading streams as clips.
	Aela::AudioStream* stream;

	if (resourceManager->obtain<AudioStream>("res/audio/streams/" + src, stream)) {
		streams[tag] = stream;
		return true;
	}

	AelaErrorHandling::consoleWindowError("Simple Audio Player",  "Did not find stream res/audio/streams/" + src);
	return false;
}

bool Aela::SimpleAudioPlayer::playAudio(std::string tag) {
	bool success = false;
	auto pair = clips.find(tag);

	if (pair != clips.end()) {
		audioPlayer->playClip(pair->second);
		success = true;
	}

	auto pair2 = streams.find(tag);

	if (pair2 != streams.end()) {
		audioPlayer->playStream(pair2->second);
		success = true;
	}

	return success;
}

bool Aela::SimpleAudioPlayer::playAudioIfNotPlaying(std::string tag) {
	if (!isPlayingClip(tag) && !isPlayingStream(tag)) {
		return playAudio(tag);
	}

	return false;
}

bool Aela::SimpleAudioPlayer::setClipVolume(std::string tag, float volume) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		return false;
	}

	pair->second->setDefaultVolume(volume);
	return audioPlayer->changeVolumeOfPlayingClip(pair->second, volume);
}

void Aela::SimpleAudioPlayer::loopClip(std::string tag, unsigned numberOfLoops, unsigned long long locationOfLoopEnd,
	unsigned long long waitTimeBeforeStart) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		AelaErrorHandling::consoleWindowWarning("Simple Audio Looper", "Could not find stream " + tag + ".");
		return;
	}

	audioLooper->loopAudioClip(pair->second, numberOfLoops, locationOfLoopEnd, waitTimeBeforeStart);
}

void Aela::SimpleAudioPlayer::loopStream(std::string tag, unsigned numberOfLoops, unsigned long long locationOfLoopEnd,
	unsigned long long waitTimeBeforeStart) {
	
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		AelaErrorHandling::consoleWindowWarning("Simple Audio Looper", "Could not find stream " + tag + ".");
		return;
	}

	audioLooper->loopAudioStream(pair->second, numberOfLoops, locationOfLoopEnd, waitTimeBeforeStart);
}

void Aela::SimpleAudioPlayer::loopClipInfinitely(std::string tag, unsigned long long locationOfLoopEnd,
	unsigned long long waitTimeBeforeStart) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		AelaErrorHandling::consoleWindowWarning("Simple Audio Looper", "Could not find stream " + tag + ".");
		return;
	}

	audioLooper->loopAudioClipInfinitely(pair->second, locationOfLoopEnd, waitTimeBeforeStart);
}

void Aela::SimpleAudioPlayer::loopStreamInfinitely(std::string tag, unsigned long long locationOfLoopEnd,
	unsigned long long waitTimeBeforeStart) {
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		AelaErrorHandling::consoleWindowWarning("Simple Audio Looper", "Could not find stream " + tag + ".");
		return;
	}

	audioLooper->loopAudioStreamInfinitely(pair->second, locationOfLoopEnd, waitTimeBeforeStart);
}

void Aela::SimpleAudioPlayer::stopClip(std::string tag) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		AelaErrorHandling::consoleWindowWarning("Simple Audio Looper", "Could not find stream " + tag + ".");
		return;
	}

	if (!audioLooper->stopClipLoop(pair->second)) {
		// The audio was not being looped but could have been played normally.
		audioPlayer->stopClip(pair->second);
	}
}

void Aela::SimpleAudioPlayer::stopStream(std::string tag) {
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		AelaErrorHandling::consoleWindowWarning("Simple Audio Looper", "Could not find stream " + tag + ".");
		return;
	}

	if (!audioLooper->stopStreamLoop(pair->second)) {
		// The audio was not being looped but could have been played normally.
		audioPlayer->stopStream(pair->second);
	}
}

void Aela::SimpleAudioPlayer::stopAllLoops() {
	audioLooper->stopAllLoops();
}

void Aela::SimpleAudioPlayer::stopEverything() {
	audioLooper->stopAllLoops();

	for (auto& pair : clips) {
		audioPlayer->stopClip(pair.second);
	}

	for (auto& pair : streams) {
		audioPlayer->stopStream(pair.second);
	}
}

void Aela::SimpleAudioPlayer::setResourceManager(ResourceManager* resourceManager) {
	this->resourceManager = resourceManager;
}

void Aela::SimpleAudioPlayer::setAudioPlayer(AudioPlayer* audioPlayer) {
	this->audioPlayer = audioPlayer;
}

void Aela::SimpleAudioPlayer::setAudioLooper(AudioLooper* audioLooper) {
	this->audioLooper = audioLooper;
}

bool Aela::SimpleAudioPlayer::setStreamVolume(std::string tag, float volume) {
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		AelaErrorHandling::consoleWindowWarning("Simple Audio Looper", "Could not find stream " + tag + ".");
		return false;
	}

	pair->second->setDefaultVolume(volume);
	bool success = audioPlayer->changeVolumeOfPlayingStream(pair->second, volume);
	return success;
}

void Aela::SimpleAudioPlayer::setAllClipVolumes(float volume) {
	for (auto& pair : clips) {
		pair.second->setDefaultVolume(volume);
		audioPlayer->changeVolumeOfPlayingClip(pair.second, volume);
	}
}

void Aela::SimpleAudioPlayer::setAllStreamVolumes(float volume) {
	for (auto& pair : streams) {
		pair.second->setDefaultVolume(volume);
		audioPlayer->changeVolumeOfPlayingStream(pair.second, volume);
	}
}

bool Aela::SimpleAudioPlayer::isPlayingClip(std::string tag) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		return false;
	}

	return audioPlayer->isPlaying(pair->second);
}

bool Aela::SimpleAudioPlayer::isPlayingStream(std::string tag) {
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		return false;
	}

	return audioPlayer->isPlaying(pair->second);
}

bool Aela::SimpleAudioPlayer::unloadAudio(std::string tag) {
	bool success = false;
	auto pair = clips.find(tag);
	if (pair != clips.end()) {
		resourceManager->unload(pair->second->getSrc());
		success = true;
	}

	auto pair2 = streams.find(tag);
	if (pair2 != streams.end()) {
		resourceManager->unload(pair2->second->getSrc());
		success = true;
	}

	return success;
}

void Aela::SimpleAudioPlayer::unloadAllClips() {
	for (auto pair : clips) {
		resourceManager->unload(pair.second->getSrc());
	}

	clips.clear();
}

void Aela::SimpleAudioPlayer::unloadAllStreams() {
	for (auto pair : streams) {
		resourceManager->unload(pair.second->getSrc());
	}

	streams.clear();
}

Aela::AudioClip* Aela::SimpleAudioPlayer::getClip(std::string tag) {
	auto pair = clips.find(tag);
	if (pair != clips.end()) {
		return pair->second;
	}
	return nullptr;
}
