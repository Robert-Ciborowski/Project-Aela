/*
* Class: AudioLooper
* Author: Robert Ciborowski
* Date: 27/01/2019
* Description: A class used to loop audio. Feel free to move this into another audio class,
*              if you think its better that way (such as AudioPlayer).
*/

#pragma once
#include <unordered_map>
#include "AudioPlayer.h"
#include "../Resource Management/ResourceManager.h"
#include "../Time/Clock.h"

namespace Aela {
	class AudioClip;

	struct AudioLoopData {
		unsigned int whichLoop;
		unsigned int numberOfLoops;
		unsigned long long timeOfStart;
		unsigned long long locationOfLoopEnd;
		unsigned long long timeWhenToStart;
		bool started;
	};

	class AudioLooper {
		public:
			AudioLooper();

			void update();

			// Note: locationOfLoopEnd and waitTimeBeforeStart are in millis.
			void loopAudioClipUsingSrc(std::string src, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopAudioStreamUsingSrc(std::string src, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopAudioClip(AudioClip* clip, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopAudioStream(AudioStream* stream, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			
			void loopAudioClipInfinitelyUsingSrc(std::string src, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopAudioStreamInfinitelyUsingSrc(std::string src, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopAudioClipInfinitely(AudioClip* clip, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopAudioStreamInfinitely(AudioStream* stream, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);

			bool stopClipLoopUsingSrc(std::string src);
			bool stopStreamLoopUsingSrc(std::string src);
			bool stopClipLoop(AudioClip* clip);
			bool stopStreamLoop(AudioStream* stream);
			void stopAllLoops();

			bool isLoopingAnyAudio();
			size_t getNumberOfCurrentLoops();
			AudioClip* getFirstLoopingClip();
			AudioLoopData* getFirstLoopDataClip();
			AudioStream* getFirstLoopingStream();
			AudioLoopData* getFirstLoopDataStream();


			void setAudioPlayer(AudioPlayer* audioPlayer);
			void setResourceManager(ResourceManager* resourceManager);
			void setClock(Clock* clock);

		private:
			std::unordered_map<AudioClip*, AudioLoopData> loopingClips;
			std::unordered_map<AudioStream*, AudioLoopData> loopingStreams;
			AudioPlayer* audioPlayer;
			ResourceManager* resourceManager;
			Clock* clock;
	};
}
