/*
* Class: Simple Audio Player
* Author: Robert Ciborowski
* Date: 02/07/2019
* Description: A class used to play audio by providing a key rather than
               a full audio resource. This was originally built for Neo-Zero
			   a while ago but was moved to Project Aela.
*/

#pragma once
#include <string>
#include <unordered_map>
#include "AudioClip.h"
#include "../Resource Management/ResourceManager.h"
#include "AudioPlayer.h"
#include "AudioLooper.h"

namespace Aela {
	class SimpleAudioPlayer {
		public:
			SimpleAudioPlayer();

			bool addAudioAsClip(const std::string tag, const std::string src);
			bool addAudioAsStream(const std::string tag, const std::string src);
			bool playAudio(const std::string tag);
			bool playAudioIfNotPlaying(const std::string tag);
			bool isPlayingClip(const std::string tag);
			bool isPlayingStream(const std::string tag);
			bool unloadAudio(const std::string tag);
			void unloadAllClips();
			void unloadAllStreams();
			AudioClip* getClip(const std::string tag);
			bool setClipVolume(const std::string tag, float volume);
			bool setStreamVolume(const std::string tag, float volume);
			void setAllClipVolumes(float volume);
			void setAllStreamVolumes(float volume);

			void loopClip(const std::string tag, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopStream(const std::string tag, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopClipInfinitely(const std::string tag, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopStreamInfinitely(const std::string tag, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void stopClip(const std::string tag);
			void stopStream(const std::string tag);
			void stopAllLoops();
			void stopEverything();

			void setResourceManager(ResourceManager* resourceManager);
			void setAudioPlayer(AudioPlayer* audioPlayer);
			void setAudioLooper(AudioLooper* audioLooper);

		private:
			std::unordered_map<std::string, Aela::AudioClip*> clips;

			// Once audio streams are added, this will be a map of streams. For now, we're using clips.
			std::unordered_map<std::string, Aela::AudioStream*> streams;

			ResourceManager* resourceManager;
			AudioPlayer* audioPlayer;
			AudioLooper* audioLooper;
	};
}
