#include "Compressor.h"
#include "zlib.h"
#include <assert.h>
#include <thread>
#include <fstream>
#include <iostream>

#define CHUNK 16384

Aela::Compressor::Compressor() {
	chunkSize = 16384;
}

Aela::Compressor::Compressor(unsigned int chunkSize) {
	this->chunkSize = chunkSize;
}

Aela::Compressor::~Compressor() {
}

Aela::CompressionError Aela::Compressor::compressStream(std::istream* source, std::ostream* dest, int level) {
	int ret, flush;
	z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);

    if (ret != Z_OK) {
        return convertZlibError(ret);
	}

	auto* in = new unsigned char[chunkSize];
	auto* out = new unsigned char[chunkSize];

    // This compresses until we reach the end of the file.
    do {
		source->read((char*) in, chunkSize);
        strm.avail_in = (uInt) source->gcount();

		if (source->bad()) {
            (void)deflateEnd(&strm);
			delete[] in;
			delete[] out;
            return convertZlibError(Z_ERRNO);
        }

        flush = source->eof() ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        // This runs deflate() on input until the output buffer is not full,
    	// and finishes compression if all of source has been read in.
        do {
            strm.avail_out = (uInt) chunkSize;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            unsigned have = chunkSize - strm.avail_out;
			
			dest->write((const char*) out, have);

			// If there is an error, do this:
            /* if () {
				(void)deflateEnd(&strm);
				std::cout << "RIP! 2";
				return Z_ERRNO;
        	*/
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);

        /* done when last data in file processed */
    } while (flush != Z_FINISH);

	if (ret != Z_STREAM_END) {
		delete[] in;
		delete[] out;
		return CompressionError::OTHER;
	}

    // assert(ret == Z_STREAM_END);

    // Clean everything up and return.
    (void) deflateEnd(&strm);
	delete[] in;
	delete[] out;
    return convertZlibError(Z_OK);
}

Aela::CompressionError Aela::Compressor::decompressStream(std::istream* source, std::ostream* dest) {
	int ret;
    unsigned have;
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);

    if (ret != Z_OK) {
        return convertZlibError(ret);
	}

	auto* in = new unsigned char[chunkSize];
	auto* out = new unsigned char[chunkSize];

    // This decompresses until deflate stream ends or the end of file is reached.
    do {
        source->read((char*) in, chunkSize);
        strm.avail_in = (uInt) source->gcount();

		if (source->bad()) {
            (void) inflateEnd(&strm);
			delete[] in;
			delete[] out;
            return convertZlibError(Z_ERRNO);
        }

        if (strm.avail_in == 0) {
            break;
		}
        strm.next_in = in;

        // This runs inflate() on the input until the output buffer is not full.
        do {
            strm.avail_out = (uInt) chunkSize;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);

			if (ret == Z_STREAM_ERROR) {
				delete[] in;
				delete[] out;
				return CompressionError::OTHER;
			}

            switch (ret) {
	            case Z_NEED_DICT:
	                ret = Z_DATA_ERROR;
	            case Z_DATA_ERROR:
	            case Z_MEM_ERROR:
	                (void)inflateEnd(&strm);
					delete[] in;
					delete[] out;
	                return convertZlibError(ret);
	            default:
            		// How did you reach this?
					break;
            }

            have = chunkSize - strm.avail_out;
            dest->write((const char*) out, have);

			// If there is an error, do this:
            /* if () {
				(void)inflateEnd(&strm);
				std::cout << "RIP! 2";
				return Z_ERRNO;
        	*/
        } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);

    // Clean everything up and return.
    (void) inflateEnd(&strm);
	delete[] in;
	delete[] out;
    return convertZlibError(ret == Z_STREAM_END || ret == Z_OK ? Z_OK : Z_DATA_ERROR);
}

int Aela::Compressor::compressTest(std::istream* source, std::ostream* dest, int level) {
	int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
		source->read((char*) in, CHUNK);
        strm.avail_in = (uInt) source->gcount();

		if (source->bad()) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }

        flush = source->eof() ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
			
			dest->write((const char*) out, have);

			// if error, do this:
            /* if () {
				(void)deflateEnd(&strm);
				std::cout << "RIP! 2";
				return Z_ERRNO;
        	*/
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

int Aela::Compressor::decompressTest(std::istream* source, std::ostream* dest) {
	int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        source->read((char*) in, CHUNK);
        strm.avail_in = (uInt) source->gcount();

		if (source->bad()) {
            (void) inflateEnd(&strm);
            return Z_ERRNO;
        }

        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            dest->write((const char*) out, have);
			
			// if error, do this:
            /* if () {
				(void)inflateEnd(&strm);
				std::cout << "RIP! 2";
				return Z_ERRNO;
        	*/
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void Aela::Compressor::setChunkSize(unsigned int chunkSize) {
	this->chunkSize = chunkSize;
}

Aela::CompressionError Aela::Compressor::convertZlibError(int error) {
	switch (error) {
		case Z_OK:
			return CompressionError::OK;
		case Z_ERRNO:
			return CompressionError::STREAM_READING_ERROR;
	    case Z_STREAM_ERROR:
	        return CompressionError::INVALID_COMPRESSION_LEVEL;
	    case Z_DATA_ERROR:
	        return CompressionError::INVALID_DEFLATE_DATA;
	    case Z_MEM_ERROR:
	        return CompressionError::OUT_OF_MEMORY;
		default:
			return CompressionError::OTHER;
	}
}
