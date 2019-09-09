#include "OGGStreamLoader.h"
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "OGGAudioStream.h"
#include "../Error Handler/ErrorHandling.h"

Aela::OGGStreamLoader::OGGStreamLoader() {
}

Aela::OGGStreamLoader::~OGGStreamLoader() {
}

bool Aela::OGGStreamLoader::load(ResourceMap& resources, std::string src) {
	OggVorbis_File* ov = new OggVorbis_File();

	FILE* input;
	fopen_s(&input, src.c_str(), "rb");

	if (ov_open_callbacks(input, ov, NULL, -1, OV_CALLBACKS_NOCLOSE) < 0) {
		delete ov;
		AelaErrorHandling::consoleWindowError("Could not open ogg stream: " + src);
		false;
	}

	if (!ov_seekable(ov)) {
		delete ov;
		AelaErrorHandling::consoleWindowError("Ogg stream's file was not seekable: " + src);
		false;
	}

	OGGAudioStream* stream = new OGGAudioStream(ov, src);
	resources.put(src, stream);
	return true;
}
