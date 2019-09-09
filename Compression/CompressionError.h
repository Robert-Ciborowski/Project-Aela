/*
* Enum: CompressionError
* Author: Robert Ciborowski
* Date: 07/11/2018
* Description: A class which represents an error during compression
*              done by the Compressor class.
*/

#pragma once

namespace Aela {
	enum class CompressionError {
		OK, STREAM_READING_ERROR, INVALID_COMPRESSION_LEVEL, INVALID_DEFLATE_DATA, OUT_OF_MEMORY, OTHER
	};
}