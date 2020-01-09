#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
#include <assert.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include "OGGClipLoader.h"
#include "AudioClip.h"
#include "../Error Handler/ErrorHandling.h"

#define BUFFER_SIZE 4096
#define BITS_PER_SAMPLE 16

Aela::OGGClipLoader::OGGClipLoader() {}

Aela::OGGClipLoader::~OGGClipLoader() {}

bool Aela::OGGClipLoader::load(ResourceMap& resources, std::string src) {
	char buffer[BUFFER_SIZE];
	std::vector<char> data;
	OggVorbis_File vf;
	int current_section;
	FILE* input;

	fopen_s(&input, src.c_str(), "rb");

	if (ov_open_callbacks(input, &vf, NULL, 0, OV_CALLBACKS_DEFAULT) < 0) {
		AelaErrorHandling::consoleWindowWarning("OGG Loader", "Input does not appear to be an Ogg bitstream: "
		+ src);
		false;
	}

	char** ptr = ov_comment(&vf, -1)->user_comments;

	while (*ptr) {
		fprintf(stderr, "%s\n", *ptr);
		++ptr;
	}

	vorbis_info* vi = ov_info(&vf, -1);
	int channels = vi->channels;
	long rate = vi->rate;
	/*fprintf(stderr, "\nBitstream is %d channel, %ldHz\n", vi->channels, vi->rate);
	fprintf(stderr, "Encoded by: %s\n\n", ov_comment(&vf, -1)->vendor);*/

	while (true) {
		long ret = ov_read(&vf, buffer, sizeof(buffer), 0, 2, 1, &current_section);

		if (ret == 0) {
			// We have reached the end of the file!
			break;
		} else if (ret < 0) {
			// Weird error in the stream?!
		} else {
			// We might have to deal with sample rate changes???
			for (size_t i = 0; i < ret; i++) {
				data.push_back(buffer[i]);
			}
		}
	}

	ov_clear(&vf);
	fclose(input);
	char* actualData = (char*) malloc(data.size());
	memcpy_s(actualData, data.size(), data.data(), data.size());
	AudioClip* clip = new AudioClip(src, actualData);
	clip->setSize((ALsizei) data.size());
	clip->setFormat(channels, BITS_PER_SAMPLE);
	clip->setSampleRate(rate);
	resources.put(src, clip);
	return true;
}
