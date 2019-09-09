#include <utility>
#include "GLTextureLoader.h"
#include "../../Utilities/strut.h"
#include "../../Utilities/Safe Functions.h"
#include "../../Error Handler/ErrorHandling.h"

using namespace Aela;

GLTextureLoader::GLTextureLoader() = default;

GLTextureLoader::~GLTextureLoader() = default;

bool Aela::GLTextureLoader::load(ResourceMap& resources, std::string src) {
	GLTexture* res = nullptr;
	loadTexture(res, src);

	resources.put(src, res);

	return true;
}

bool GLTextureLoader::loadTexture(GLTexture*& result, std::string src) {
	// This creates an OpenGL texture.
	GLuint modelTextureID;
	glGenTextures(1, &modelTextureID);

	// This tells OpenGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_2D, modelTextureID);

	unsigned int imageWidth = 0, imageHeight = 0;

	std::string format = src.substr(src.size() - 4, 4);

	// Note: I've seen some programs output image files as ".PNG" instead of ".png". Why would you do that? No clue.
	// Don't believe me? Try using Windows' Snipping Tool.
	if (format == ".dds" || format == ".DDS" || format == ".bmp" || format == ".BMP") {
		std::ifstream in(src, std::ios::binary);
		if (!isValid(in)) {
			AelaErrorHandling::consoleWindowError("Aela Texture Loader", "The file " + src + " could not load.");
			return false;
		}
		if (format == ".dds" || format == ".DDS") {
			if (!loadDDSToBoundId(in, GL_TEXTURE_2D, &imageWidth, &imageHeight)) {
				in.close();
				AelaErrorHandling::consoleWindowError("Aela Texture Loader", "The file " + src + " could not load.");
				return false;
			}
		} else if (format == ".bmp" || format == ".BMP") {
			if (!loadBMPToBoundId(in, GL_TEXTURE_2D, &imageWidth, &imageHeight)) {
				in.close();
				AelaErrorHandling::consoleWindowError("Aela Texture Loader", "The file " + src + " could not load.");
				return false;
			}
		}
		in.close();
	} else if (format == ".png" || format == ".PNG") {
		// Libpng is a c library. It does not use streams.
		if (!loadPNGUsingFILE(src, GL_TEXTURE_2D, &imageWidth, &imageHeight)) {
			AelaErrorHandling::consoleWindowError("Aela Texture Loader", "The file " + src + " could not be read.");
		}
	} else {
		AelaErrorHandling::consoleWindowError("Aela Texture Loader", "The file " + src + " uses an unsupported format.");
	}

	result = new GLTexture(src, modelTextureID);

	// I'm removing this feature for now, since it's rather important to be consistent throughout Aela in resource conventions.
	// In addition, this currently does not save any memory since an extra property was added to every resource.
	// Please talk to me about re-adding this feature if you need it.

	/*// This checks to see if the resource can use the "~" shortcut for its src to signify that the resource is using the
	// commonly excepted src for its resource type. Doing this saves memory.
	if (src.substr(0, defaultTexturePath.size()) == defaultTexturePath) {
		result->setSource("~" + src.substr(defaultTexturePath.size(), src.size()));
	} else {
		result->setSource(src);
	}*/
	result->setDimensions(0, 0, imageWidth, imageHeight);
	return true;
}

bool GLTextureLoader::loadDDSToBoundId(std::ifstream& in, GLenum target) {
	return loadDDSToBoundId(in, target, nullptr, nullptr);
}

bool GLTextureLoader::loadDDSToBoundId(std::ifstream& in, GLenum target, unsigned int* width, unsigned int* height) {
	// using char[] for speed
	unsigned char textureHeader[AELA_RESOURCE_DDS_HEADER_SIZE];

	// read the file header
	if (!in.read(reinterpret_cast<char*>(textureHeader), AELA_RESOURCE_DDS_HEADER_SIZE)) {
		return false;
	}

	// make sure we are reading a texture file
	if (strncmp((char*) textureHeader, AELA_RESOURCE_DDS_HEADER_START, 4) != 0) {
		return false;
	}

	unsigned int imageHeight = *(unsigned int*) &(textureHeader[12]), imageWidth = *(unsigned int*) &(textureHeader[16]),
		linearSize = *(unsigned int*) &(textureHeader[20]), mipMapAmount = *(unsigned int*) &(textureHeader[28]),
		fourCCType = *(unsigned int*) &(textureHeader[84]), bufferSize;

	unsigned int heightToReturn = imageHeight, widthToReturn = imageWidth;

	// calculate texture size, including mip-maps
	bufferSize = (mipMapAmount > 1) ? (linearSize * 2) : linearSize;

	// read the contents of the file.
	auto* buffer = new unsigned char[bufferSize * sizeof(unsigned char)];
	in.read(reinterpret_cast<char*>(buffer), bufferSize);

	// determine the texture format
	unsigned int format;
	switch ((CompressionAlgorithms) fourCCType) {
		case CompressionAlgorithms::FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case CompressionAlgorithms::FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case CompressionAlgorithms::FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			delete[] buffer;
			return false;
	}

	// set the pixel storage mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// load the mipmaps
	for (unsigned int level = 0; level < mipMapAmount && (imageWidth || imageHeight); level++) {
		unsigned int size = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * blockSize;
		glCompressedTexImage2D(target, level, format, imageWidth, imageHeight, 0, size, buffer + offset);

		// This selects a filtering to use for the texture. Maybe we should add a feature in which the user
		// of the engine can select a type of filtering to use.
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		offset += size;
		imageWidth /= 2;
		imageHeight /= 2;

		// ensure the texture has dimensions that are powers of two
		if (imageWidth < 1) {
			imageWidth = 1;
		}
		if (imageHeight < 1) {
			imageHeight = 1;
		}
	}
	delete[] buffer;

	if (width != nullptr) {
		*width = widthToReturn;
	}
	if (height != nullptr) {
		*height = heightToReturn;
	}

	return true;
}

bool Aela::GLTextureLoader::loadBMPToBoundId(std::ifstream& in, GLenum target) {
	return loadBMPToBoundId(in, target, nullptr, nullptr);
}

bool Aela::GLTextureLoader::loadBMPToBoundId(std::ifstream& in, GLenum target, unsigned int* width,
	unsigned int* height) {
	// These are variables for the types of data that are read from the file.
	unsigned char header[AELA_RESOURCE_BMP_HEADER_SIZE];
	unsigned int dataPosition;
	unsigned int bufferSize;
	unsigned int imageWidth, imageHeight;
	unsigned char* buffer, *flippedBuffer;
	unsigned int bytesPerPixel;

	// read the file header
	if (!in.read(reinterpret_cast<char*>(header), AELA_RESOURCE_BMP_HEADER_SIZE)) {
		return false;
	}

	bool validFile = true;

	if (header[0] != 'B' || header[1] != 'M') {
		validFile = false;
	}

	// This makes sure that the file is a 24bpp or 32bpp file (bits per pixel).
	if (*(int*)&(header[0x1E]) != 0) {
		validFile = false;
	}

	// TODO determine if changing this to unsigned int, from int, crashes the program
	bytesPerPixel = *(unsigned int*)&(header[0x1C]) / 8;

	if (!(bytesPerPixel == 3 || bytesPerPixel == 4)) {
		validFile = false;
	}

	if (!validFile) {
		AelaErrorHandling::consoleWindowError("Project Aela's BMP loader", (std::string) "The BMP does not contain a" +
			"proper bits per pixel value or is corrupt!");
		return false;
	}

	dataPosition = *(unsigned int*)&(header[10]);
	bufferSize = *(unsigned int*)&(header[34]);
	imageWidth = *(unsigned int*)&(header[18]);
	imageHeight = *(unsigned int*)&(header[22]);

	// If the BMP file is not formatted properly, these statements will trigger.

	if (bufferSize == 0) {
		// One byte per RGB value (hence the 24 or 32 bpp).
		bufferSize = imageWidth * imageHeight * bytesPerPixel;
	}

	if (dataPosition == 0) {
		// The BMP header should end at 53.
		dataPosition = AELA_RESOURCE_BMP_HEADER_SIZE;
	}

	// This grabs the data from the file. Note that BMPs are stored upside down due to some debate
	// at IBM many years ago.
	buffer = new unsigned char[bufferSize], flippedBuffer = new unsigned char[bufferSize];

	in.read(reinterpret_cast<char*>(buffer), bufferSize);

	for (size_t y = 0; y < imageHeight; y++) {
		memcpy(&flippedBuffer[y * imageWidth * bytesPerPixel], &buffer[(imageHeight - y - 1) * imageWidth
			* bytesPerPixel], imageWidth * bytesPerPixel);
	}

	// This sets up the OpenGL texture.
	if (bytesPerPixel == 3) {
		// The bytes per pixel is 3!
		glTexImage2D(target, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, flippedBuffer);
	} else {
		// The bytes per pixel is 4! Idk if this actually works.
		glTexImage2D(target, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, flippedBuffer);
	}
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// This selects a filtering to use for the texture. Maybe we should add a feature in which the user
	// of the engine can select a type of filtering to use.
	glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(target);

	if (width != nullptr) {
		*width = imageWidth;
	}
	if (height != nullptr) {
		*height = imageHeight;
	}

	// Since the data is saved to the texture, the data array should be deleted!
	delete[] buffer;
	delete[] flippedBuffer;

	return true;
}

bool GLTextureLoader::loadDDSUsingFILE(std::string src, GLenum target) {
	return loadDDSUsingFILE(std::move(src), target, nullptr, nullptr);
}

// This loads a texture using FILE. If you're wondering why the first parameter is the src rather than a FILE* (similar to how
// loadTexture's first parameter is an ifstream&), it's because you'd have to call ResourceLoader::open before using this function,
// this would mean having to pass a nullptr of FILE* into ResourceLoader::open, which breaks everything. Go ahead, try it, you'll see.
bool GLTextureLoader::loadDDSUsingFILE(std::string src, GLenum target, unsigned int* width, unsigned int* height) {
	FILE* in;

	// using char[] for speed
	unsigned char textureHeader[AELA_RESOURCE_DDS_HEADER_SIZE];

	// This will try to open the DDS file.
	fopen_safe(&in, src.c_str(), "rb");
	if (in == nullptr) {
		AelaErrorHandling::windowError("Aela DDS Loader", "A DDS file was not found.");
		return false;
	}

	// read the file header
	fread(textureHeader, 1, AELA_RESOURCE_DDS_HEADER_SIZE, in);

	// make sure we are reading a texture file
	if (strncmp((char*) textureHeader, AELA_RESOURCE_DDS_HEADER_START, 4) != 0) {
		return false;
	}

	unsigned int imageHeight = *(unsigned int*) &(textureHeader[12]);
	unsigned int imageWidth = *(unsigned int*) &(textureHeader[16]);
	unsigned int linearSize = *(unsigned int*) &(textureHeader[20]);
	unsigned int mipMapAmount = *(unsigned int*) &(textureHeader[28]);
	unsigned int fourCCType = *(unsigned int*) &(textureHeader[84]);
	unsigned int bufferSize;

	// calculate texture size, including mip-maps
	bufferSize = (mipMapAmount > 1) ? (linearSize * 2) : linearSize;

	// read the contents of the file.
	auto* buffer = new unsigned char[bufferSize * sizeof(unsigned char)];
	fread(buffer, 1, bufferSize, in);

	// determine the texture format
	unsigned int format;
	switch ((CompressionAlgorithms) fourCCType) {
	case CompressionAlgorithms::FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case CompressionAlgorithms::FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case CompressionAlgorithms::FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		delete[] buffer;
		return false;
	}

	// set the pixel storage mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// load the mipmaps
	for (unsigned int level = 0; level < mipMapAmount && (imageWidth || imageHeight); level++) {
		unsigned int size = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * blockSize;
		glCompressedTexImage2D(target, level, format, imageWidth, imageHeight, 0, size, buffer + offset);
		offset += size;
		imageWidth /= 2;
		imageHeight /= 2;

		// ensure the texture has dimensions that are powers of two
		if (imageWidth < 1) {
			imageWidth = 1;
		}
		if (imageHeight < 1) {
			imageHeight = 1;
		}

	}
	delete[] buffer;

	if (width != nullptr) {
		*width = imageWidth;
	}
	if (height != nullptr) {
		*height = imageHeight;
	}
	return true;
}

bool Aela::GLTextureLoader::loadBMPUsingFILE(std::string src, GLenum target) {
	return loadBMPUsingFILE(std::move(src), target, nullptr, nullptr);
}

bool Aela::GLTextureLoader::loadBMPUsingFILE(std::string src, GLenum target, unsigned int* width, unsigned int* height) {
	unsigned char header[54];
	unsigned int dataPosition;
	unsigned int imageSize;
	unsigned int imageWidth, imageHeight;
	unsigned char* data;

	// This opens the file with the "read - binary" mode.
	FILE* file;
	fopen_safe(&file, src.c_str(), "rb");
	if (!file) {
		AelaErrorHandling::consoleWindowError("Project Aela's BMP loader", src + " could not be found.");
		return false;
	}

	bool validFile = true;

	if (fread(header, 1, 54, file) != 54) {
		validFile = false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		validFile = false;
	}

	// This makes sure that the file is a 24bpp file (bits per pixel).
	if (*(int*)&(header[0x1E]) != 0) {
		validFile = false;
	}

	if (*(int*)&(header[0x1C]) != 24) {
		validFile = false;
	}

	if (!validFile) {
		AelaErrorHandling::consoleWindowError("Project Aela's BMP loader", src + " does not contain a proper bits per pixel value or is corrupt!");
		return false;
	}

	dataPosition = *(unsigned int*)&(header[10]);
	imageSize = *(unsigned int*)&(header[34]);
	imageWidth = *(unsigned int*)&(header[18]);
	imageHeight = *(unsigned int*)&(header[22]);

	// If the BMP file is not formatted properly, these statements will trigger.

	if (imageSize == 0) {
		// One byte per RGB value (hence the 24 bpp).
		imageSize = imageWidth * imageHeight * 3;
	}

	if (dataPosition == 0) {
		// The BMP header should end at 53.
		dataPosition = 54;
	}

	// This grabs the data from the file.
	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	// This sets up the OpenGL texture.
	glTexImage2D(target, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(target);

	// Since the data is saved to the texture, the data array should be deleted!
	delete[] data;

	if (width != nullptr) {
		*width = imageWidth;
	}
	if (height != nullptr) {
		*height = imageHeight;
	}

	return true;
}

bool Aela::GLTextureLoader::loadPNGToBuffer(std::string src, png_bytep** rows, unsigned int* width,
	unsigned int* height, unsigned int* bytesPerPixel) {
	png_structp pngPtr;
	png_infop infoPtr;

	unsigned char header[AELA_RESOURCE_PNG_HEADER_SIZE];
	png_bytep* rowBuffer;

	// This opens the file.
	FILE *file;
	fopen_safe(&file, src.c_str(), "rb");
	if (!file) {
		AelaErrorHandling::consoleWindowError("Project Aela's PNG loader", src + " could not be found.");
		return false;
	}

	// This reads the header.
	fread(header, 1, 8, file);
	if (png_sig_cmp((png_bytep) header, 0, 8)) {
		AelaErrorHandling::consoleWindowError("Project Aela's PNG loader", src + " is not a valid PNG file.");
		return false;
	}

	// This initialises the PNG struct.
	pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

	if (!pngPtr) {
		AelaErrorHandling::consoleWindowError("Project Aela's PNG loader", (std::string) "There was a problem with" +
			" setting up a PNG. " + src + " may be corrupt.");
		return false;
	}

	// This sets up the struct that contains information on the PNG.
	infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr) {
		AelaErrorHandling::consoleWindowError("Project Aela's PNG loader", (std::string) "There was a problem with" +
			" setting up a PNG's info. " + src + " may be corrupt.");
		return false;
	}

	// This initialises some other IO stuff.
	if (setjmp(png_jmpbuf(pngPtr))) {
		AelaErrorHandling::consoleWindowError("Project Aela's PNG loader", (std::string) "There was a problem with" +
			" setting up a PNG's IO. " + src + " may be corrupt.");
		return false;
	}

	png_init_io(pngPtr, file);
	png_set_sig_bytes(pngPtr, 8);
	png_read_info(pngPtr, infoPtr);

	*width = png_get_image_width(pngPtr, infoPtr);
	*height = png_get_image_height(pngPtr, infoPtr);

	// unsigned int numberOfPasses = png_set_interlace_handling(pngPtr);
	png_set_interlace_handling(pngPtr);
	png_read_update_info(pngPtr, infoPtr);

	// This actually reads the PNG content.
	if (setjmp(png_jmpbuf(pngPtr))) {
		AelaErrorHandling::consoleWindowError("Project Aela's PNG loader", (std::string) "There was a problem with" +
			" reading a PNG's content. " + src + " may be corrupt.");
		return false;
	}

	rowBuffer = new png_bytep[*height];

	for (unsigned int y = 0; y < *height; y++) {
		rowBuffer[y] = new png_byte[png_get_rowbytes(pngPtr, infoPtr)];
	}

	png_read_image(pngPtr, rowBuffer);

	fclose(file);

	if (png_get_color_type(pngPtr, infoPtr) == PNG_COLOR_TYPE_RGB) {
		*bytesPerPixel = 3;
	} else if (png_get_color_type(pngPtr, infoPtr) == PNG_COLOR_TYPE_RGBA) {
		*bytesPerPixel = 4;
	} else {
		AelaErrorHandling::consoleWindowError("Project Aela's PNG loader", src + (std::string) " is in an incorrect"
			+ " format. Try RGB or RGBA.");
		for (size_t i = 0; i < *height; i++) {
			delete[] rowBuffer[i];
		}
		delete[] rowBuffer;
		return false;
	}

	*rows = rowBuffer;

	return true;
}

bool Aela::GLTextureLoader::loadPNGUsingFILE(std::string src, GLenum target) {
	return loadPNGUsingFILE(std::move(src), target, nullptr, nullptr);
}

bool Aela::GLTextureLoader::loadPNGUsingFILE(std::string src, GLenum target, unsigned int* width, unsigned int* height) {
	png_bytep* rows = nullptr;
	png_byte* buffer;
	unsigned int imageWidth = 0, imageHeight = 0;
	unsigned int bytesPerPixel = 0;
	
	if (!loadPNGToBuffer(std::move(src), &rows, &imageWidth, &imageHeight, &bytesPerPixel)) {
		return false;
	}

	buffer = new png_byte[imageWidth * imageHeight * bytesPerPixel];

	for (unsigned int y = 0; y < imageHeight; y++) {
		png_byte* row = rows[y];
		for (unsigned int x = 0; x < imageWidth; x++) {
			png_byte* ptr = &(row[x * bytesPerPixel]);
			unsigned int index = (y * imageWidth + x) * bytesPerPixel;

			for (unsigned int i = 0; i < bytesPerPixel; i++) {
				buffer[index + i] = (char) ptr[i];
			}
		}
	}

	// This sets up the OpenGL texture.
	if (bytesPerPixel == 3) {
		// The bytes per pixel is 3!
		glTexImage2D(target, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	} else {
		// The bytes per pixel is 4! Idk if this actually works.
		glTexImage2D(target, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	}
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// This selects a filtering to use for the texture. Maybe we should add a feature in which the user
	// of the engine can select a type of filtering to use.
	glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(target);

	if (width != nullptr) {
		*width = imageWidth;
	}
	if (height != nullptr) {
		*height = imageHeight;
	}

	for (unsigned int y = 0; y < imageHeight; y++) {
		delete rows[y];
	}

	delete[] rows;
	delete[] buffer;
	return true;
}
