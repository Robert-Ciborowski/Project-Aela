/*
* Class: AudioPlayer
* Author: Robert Ciborowski
* Date: 28/06/2017
* Description: Represents a class that facilitates playing all in-game audio.
*/

#pragma once
#include <al.h>
#include <alc.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include "AudioListener.h"
#include "AudioClip.h"
#include "AudioStream.h"
#include "AudioBuffer.h"
//#include "ctpl_stl.h"

namespace Aela {
	class AudioPlayer {
		public:
			AudioPlayer();
			~AudioPlayer();

			bool init();
			void die();

			void playClip(AudioClip* clip);
			void stopClip(AudioClip* clip);
			bool isPlaying(AudioClip* clip);
			bool changeVolumeOfPlayingClip(AudioClip* clip, float volume);
			void setGeneralVolume(int generalVolume);
			int getGeneralVolume();

			void playStream(AudioStream* stream);
			void stopStream(AudioStream* stream);
			bool isPlaying(AudioStream* stream);
			bool changeVolumeOfPlayingStream(AudioStream* stream, float volume);

			void update();

			AudioListener getListener();

		private:
			ALCcontext* context = nullptr;
			ALCdevice* audioDevice = nullptr;

			void clearErrors();

			// These have to be pointers so that this class is Python-exposable.
			std::thread* backgroundThread;
			std::mutex* clipMutex;
			std::mutex* streamMutex;
			std::mutex* closeThreadMutex;

			std::vector<AudioClip*> playingClips;
			std::vector<AudioStream*> playingStreams;

			// This is temporary stuff until the pools are added. I just want to listen
			// to audio while doing other things.
			std::vector<AudioBuffer> playingBuffers;
			bool closeThread = false;

			int generalVolume = 100;
	};
}
