#include "OGGAudioStream.h"
#include "../Utilities/alut.h"

#define QUEUE_SIZE 12
#define BITS_PER_SAMPLE 16
#define SAMPLES_PER_DATA_CHUNK 4096
#define BYTES_TO_SAMPLE_DIVISOR 4

Aela::OGGAudioStream::OGGAudioStream(OggVorbis_File* ov, std::string src) : AudioStream(src) {
	this->ov = ov;
	vorbis_info* vi = ov_info(ov, -1);
	setFormat(vi->channels, BITS_PER_SAMPLE);
	sampleRate = vi->rate;
	lengthInSeconds = ov_time_total(ov, -1);
	lengthInNanos = (unsigned long long) (lengthInSeconds * 1000000000ll);
}

Aela::OGGAudioStream::~OGGAudioStream() {
	ov_clear(ov);
	delete ov;
}

void Aela::OGGAudioStream::loadDataIntoBuffer(ALuint buffer) {
	char data[SAMPLES_PER_DATA_CHUNK];
	ov_pcm_seek(ov, samplesLoaded);
	int current_section;
	long ret = ov_read(ov, data, SAMPLES_PER_DATA_CHUNK, 0, 2, 1, &current_section);

	if (ret == 0) {
		// We have reached the end of the file!
		playing = false;
		return;
	} else if (ret < 0) {
		// Weird error in the stream?!
		playing = false;
		return;
	} else {
		// Success! But we might have to deal with sample rate changes???

		// ret = # of bytes read
		// samplesLoaded = # of samples
		samplesLoaded += ret / BYTES_TO_SAMPLE_DIVISOR;
	}

	alDoWithErrorCheck_void(alBufferData(buffer, format, data, ret, sampleRate));
}