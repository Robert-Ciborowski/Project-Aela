/*
* Class: OpenGL Texture Loader
* Author: Robert Ciborowski
* Date: 2017?
* Description: A class used to load an image.
*/

#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include <fstream>
#include "png.h"
#include "../../Resource Management/ResourceLoader.h"
#include "../../2D/Texture/GLTexture.h"

#define AELA_RESOURCE_DDS_HEADER_SIZE 128
#define AELA_RESOURCE_DDS_HEADER_START "DDS "
#define AELA_RESOURCE_BMP_HEADER_SIZE 54
#define AELA_RESOURCE_PNG_HEADER_SIZE 8

namespace Aela {
	enum class CompressionAlgorithms : unsigned int {
		FOURCC_DXT1 = 0x31545844, FOURCC_DXT3 = 0x33545844, FOURCC_DXT5 = 0x35545844
	};

	class GLTextureLoader : public ResourceLoader {
		public:
			GLTextureLoader();
			virtual ~GLTextureLoader();

			virtual bool load(ResourceMap& resources, std::string src);

		protected:
			bool loadTexture(GLTexture*& result, std::string src);
			bool loadDDSToBoundId(std::ifstream& in, GLenum target);
			bool loadDDSToBoundId(std::ifstream& in, GLenum target, unsigned int* width, unsigned int* height);
			bool loadBMPToBoundId(std::ifstream& in, GLenum target);
			bool loadBMPToBoundId(std::ifstream& in, GLenum target, unsigned int* width, unsigned int* height);

			// Since you can't use a stream with libpng, you must use FILE.
			bool loadPNGUsingFILE(std::string src, GLenum target);
			bool loadPNGUsingFILE(std::string src, GLenum target, unsigned int* width, unsigned int* height);

			// These functions let you load different file formats using FILE. They're kind of unnecessary.
			bool loadDDSUsingFILE(std::string src, GLenum target);
			bool loadDDSUsingFILE(std::string src, GLenum target, unsigned int* width, unsigned int* height);
			bool loadBMPUsingFILE(std::string src, GLenum target);
			bool loadBMPUsingFILE(std::string src, GLenum target, unsigned int* width, unsigned int* height);

			// These can be useful for subclasses that do not want to immediately turn the image data into
			// an OpenGL texture but want to use the data for their own purposes (such as GLSpriteSheetLoader).
			// It would be nice if we could have this type of function for every file format, if possible.
			// Remember to delete each element in rows as well as rows itself once you're done with it!
			bool loadPNGToBuffer(std::string src, png_bytep** rows, unsigned int* width, unsigned int* height, unsigned int* bytesPerPixel);
	};
}
