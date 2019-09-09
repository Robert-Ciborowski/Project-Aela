/*
* Class: Audio Stream
* Author: Robert Ciborowski
* Date: 15/07/2019
* Description: Represents an audio stream.
*/

#pragma once

#include "../Resource Management/Resource.h"
#include "AudioBuffer.h"
#include "../Time/Clock.h"
#include <vector>

namespace Aela {
	class AudioStream : public Resource {
		friend class AudioPlayer;
	public:
		AudioStream(std::string src);
		virtual ~AudioStream();

		void update();
		void pause();
		void unpause();
		void stop();
		void seekToTime(unsigned long long timeInNanos);
		bool isPlaying();

		ALenum getFormat();
		ALsizei getSampleRate();
		float getDefaultVolume();
		float getVolume();

		void setFormat(ALenum format);
		void setFormat(int channelCount, int bitsPerSample);
		void setSampleRate(ALsizei sampleRate);
		void setDefaultVolume(float defaultVolume);

	protected:
		std::vector<ALuint> buffers;

		ALenum format;
		ALsizei sampleRate;
		ALuint source = NULL;
		ALint sourceState;
		float defaultVolume = 1.0f;
		float volume = 1.0f;
		unsigned long long progress = 0;
		int currentBuffer = 0;
		unsigned long long samplesLoaded = 0;
		unsigned long long lengthInNanos = 1;
		double lengthInSeconds;
		bool paused = false;
		bool playing = false;

		virtual void loadDataIntoBuffer(ALuint buffer);
		

	private:
		Clock clock;
		ALint buffersProcessed;
		unsigned long long timeOfLastUpdate = 0;

		void start(int generalVolume);
		void setupQueues();
		void currentBufferIsFinished();
		void deleteSourceAndBuffers();
	};
}