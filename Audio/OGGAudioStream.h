/*
* Class: OGG Audio Stream
* Author: Robert Ciborowski
* Date: 25/07/2019
* Description: Represents an ogg vorbis audio stream.
*/

#pragma once
#include "AudioStream.h"
#include "../Resource Management/Resource.h"
#include "AudioBuffer.h"
#include "../Time/Clock.h"
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
#include <vector>

namespace Aela {
	class OGGAudioStream : public AudioStream {
		friend class AudioPlayer;
	public:
		OGGAudioStream(OggVorbis_File* ov, std::string src);
		virtual ~OGGAudioStream();

	protected:
		void loadDataIntoBuffer(ALuint buffer);

	private:
		OggVorbis_File* ov;
	};
}