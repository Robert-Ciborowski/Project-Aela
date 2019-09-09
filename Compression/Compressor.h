/*
* Class: Compressor
* Author: Robert Ciborowski
* Date: 07/11/2018
* Description: A class used to compress streams and files.
*/

#pragma once
#include "CompressionError.h"
#include <ostream>

namespace Aela {
	class Compressor {
		public:
			Compressor();
			Compressor(unsigned int chunkSize);
			~Compressor();

			// This performs (de)compression on streams.
			CompressionError compressStream(std::istream *source, std::ostream *dest, int level);
			CompressionError decompressStream(std::istream *source, std::ostream *dest);

			int compressTest(std::istream *source, std::ostream *dest, int level);
			int decompressTest(std::istream *source, std::ostream *dest);

			// If you would like me to add in compression for FILE, I can do that.
			// I have the code. However, I thought that excluding the functionality
			// would force the user of the library to be good and use streams.

			void setChunkSize(unsigned int chunkSize);

		private:
			unsigned int chunkSize;

			CompressionError convertZlibError(int error);
	};
}
