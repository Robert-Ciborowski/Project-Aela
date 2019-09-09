/*
* Class: WAVE Clip Loader
* Author: Robert Ciborowski
* Date: 11/07/2019
* Description: Loads .wav files as audio clips.
*/

#pragma once

#include "../Resource Management/ResourceLoader.h"
#include "../Resource Management/ResourceMap.h"

#define CHUNK_HEADER_SIZE 12
#define SUBCHUNK1_HEADER_SIZE 8
#define SUBCHUNK2_HEADER_SIZE 8

#define RIFF_CHUNK_ID "RIFF"
#define SUBCHUNK1_ID "fmt "
#define SUBCHUNK2_ID "data"

#define WAVE_FORMAT "WAVE"

#define PCM_SUBCHUNK1_SIZE 16
#define PCM_FORMAT_CODE 1

namespace Aela {
	class WAVEClipLoader : public ResourceLoader {
	public:
		WAVEClipLoader();
		virtual ~WAVEClipLoader();

		virtual bool load(ResourceMap& resources, std::string src);
	};
}