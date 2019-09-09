#include <stdio.h>
// #include <stdlib.h>
#include <string>
#include "../Error Handler/ErrorHandling.h"
#include "../Old Garbage/texture.hpp"
#include "../Utilities/Safe Functions.h"
#include <iostream>
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <cstring>
#include <GL/glew.h>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

// This function should probably be moved somewhere else. Maybe it should be moved into
// a header file called "useful_functions.h". For now, it exists here.
// Edit: this function isn't even being used in this file anymore. I'm keeping it here
// so that it may be moved elsewhere.
std::string getPartOfString(std::string string, unsigned int start, unsigned int length) {
	std::string returnValue = "";
	for (unsigned int i = start; i < start + length; i++) {
		returnValue += string.at(i);
	}
	return returnValue;
}

// This is the function for loading DDS files. It uses C functions and unsigned chars since
// that's what OpenGL wants to use. Using std::string and std::fstream would make things more complex.
GLuint loadDDSToGLuint(std::string filePath) {
	unsigned char fileHeader[124];
	FILE* imageFile;

	// This will try to open the DDS file.
	fopen_safe(&imageFile, filePath.c_str(), "rb");
	if (imageFile == NULL) {
		AelaErrorHandling::windowError("Aela DDS Loader", "A DDS file " + filePath + " was not found.");
		return 0;
	}

	// This will check the file to see if it begins with "DDS ".
	// If this file header does not exist, the file is not a DDS file.
	char DDSText[4];
	fread(DDSText, 1, 4, imageFile);
	if (strncmp(DDSText, "DDS ", 4) != 0) {
		fclose(imageFile);
		return 0;
	}

	// This will receive the rest of the header.
	fread(&fileHeader, 124, 1, imageFile);

	unsigned int imageHeight = *(unsigned int*) &(fileHeader[8]);
	unsigned int imageWidth = *(unsigned int*) &(fileHeader[12]);
	unsigned int linearSize = *(unsigned int*) &(fileHeader[16]);
	unsigned int mipMapAmount = *(unsigned int*) &(fileHeader[24]);
	unsigned int fourCCType = *(unsigned int*) &(fileHeader[80]);
	unsigned char* buffer;
	unsigned int bufferSize;
	
	// This checks to see how large the bufferSize will be, including all mip-maps.
	bufferSize = mipMapAmount > 1 ? linearSize * 2 : linearSize;

	// This allocates memory for the buffer using malloc() and reads the rest of the file.
	buffer = (unsigned char*) malloc(bufferSize * sizeof(unsigned char));
	fread(buffer, 1, bufferSize, imageFile);
	
	// This closes the file.
	fclose(imageFile);

	// This finds the proper format.
	unsigned int format;
	switch (fourCCType) {
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// This creates an OpenGL texture.
	GLuint modelTextureID;
	glGenTextures(1, &modelTextureID);

	// This tells openGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_2D, modelTextureID);

	// This sets the pixel storage mode.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// This loads the mipmaps.
	for (unsigned int level = 0; level < mipMapAmount && (imageWidth || imageHeight); level++) {
		unsigned int size = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, imageWidth, imageHeight, 0, size, buffer + offset);
		offset += size;
		imageWidth /= 2;
		imageHeight /= 2;

		// If these if statements are triggered, the textures do not have power of two dimensions. Eww.
		if (imageWidth < 1) {
			imageWidth = 1;
		}
		if (imageHeight < 1) {
			imageHeight = 1;
		}

	}
	free(buffer);
	return modelTextureID;
}

void loadDDSToCubemapSide(GLuint* cubeMap, int side, std::string filePath) {
	unsigned char fileHeader[124];
	FILE* imageFile;

	// This will try to open the DDS file.
	fopen_safe(&imageFile, filePath.c_str(), "rb");
	if (imageFile == NULL) {
		AelaErrorHandling::windowError("Aela DDS Loader", "A DDS file " + filePath + " was not found.");
		return;
	}

	// This will check the file to see if it begins with "DDS ".
	// If this file header does not exist, the file is not a DDS file.
	char DDSText[4];
	fread(DDSText, 1, 4, imageFile);
	if (strncmp(DDSText, "DDS ", 4) != 0) {
		fclose(imageFile);
		return;
	}

	// This will receive the rest of the header.
	fread(&fileHeader, 124, 1, imageFile);

	unsigned int imageHeight = *(unsigned int*) &(fileHeader[8]);
	unsigned int imageWidth = *(unsigned int*) &(fileHeader[12]);
	unsigned int linearSize = *(unsigned int*) &(fileHeader[16]);
	unsigned int mipMapAmount = *(unsigned int*) &(fileHeader[24]);
	unsigned int fourCCType = *(unsigned int*) &(fileHeader[80]);
	unsigned char* buffer;
	unsigned int bufferSize;

	// This checks to see how large the bufferSize will be, including all mip-maps.
	bufferSize = mipMapAmount > 1 ? linearSize * 2 : linearSize;

	// This allocates memory for the buffer using malloc() and reads the rest of the file.
	buffer = (unsigned char*) malloc(bufferSize * sizeof(unsigned char));
	fread(buffer, 1, bufferSize, imageFile);

	// This closes the file.
	fclose(imageFile);

	// This finds the proper format.
	unsigned int format;
	switch (fourCCType) {
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return;
	}

	// This tells openGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMap);

	// This sets the pixel storage mode.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// This loads the mipmaps.
	for (unsigned int level = 0; level < mipMapAmount && (imageWidth || imageHeight); level++) {
		unsigned int size = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, level, format, imageWidth, imageHeight, 0, size, buffer + offset);
		offset += size;
		imageWidth /= 2;
		imageHeight /= 2;

		// If these if statements are triggered, the textures do not have power of two dimensions. Eww.
		if (imageWidth < 1) {
			imageWidth = 1;
		}
		if (imageHeight < 1) {
			imageHeight = 1;
		}
	}
	free(buffer);
}

// I (Julian) commented this out because I don't want to fix code we will never need again.
// Uncomment this to see why it doesn't work.
/*GLTexture loadDDSToTexture(std::string filePath) {
	unsigned char fileHeader[124];
	FILE* imageFile;

	// This will try to open the DDS file.
	fopen_safe(&imageFile, filePath.c_str(), "rb");
	if (imageFile == NULL) {
		AelaErrorHandling::windowError("Aela DDS Loader", "A DDS file that the program tried to read was not found.");
		return NULL;
	}

	// This will check the file to see if it begins with "DDS ".
	// If this file header does not exist, the file is not a DDS file.
	char DDSText[4];
	fread(DDSText, 1, 4, imageFile);
	if (strncmp(DDSText, "DDS ", 4) != 0) {
		fclose(imageFile);
		return NULL;
	}

	// This will recieve the rest of the header.
	fread(&fileHeader, 124, 1, imageFile);

	unsigned int imageHeight = *(unsigned int*) &(fileHeader[8]);
	unsigned int imageWidth = *(unsigned int*) &(fileHeader[12]);
	unsigned int linearSize = *(unsigned int*) &(fileHeader[16]);
	unsigned int mipMapAmount = *(unsigned int*) &(fileHeader[24]);
	unsigned int fourCCType = *(unsigned int*) &(fileHeader[80]);
	unsigned char* buffer;
	unsigned int bufferSize;

	// This checks to see how large the bufferSize will be, including all mip-maps.
	bufferSize = mipMapAmount > 1 ? linearSize * 2 : linearSize;

	// This allocates memory for the buffer using malloc() and reads the rest of the file.
	buffer = (unsigned char*) malloc(bufferSize * sizeof(unsigned char));
	fread(buffer, 1, bufferSize, imageFile);

	// This closes the file.
	fclose(imageFile);

	// This finds the proper format.
	unsigned int format;
	switch (fourCCType) {
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			return NULL;
	}

	// This creates an OpenGL texture.
	GLuint modelTextureID;
	glGenTextures(1, &modelTextureID);

	// This tells openGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_2D, modelTextureID);

	// This sets the pixel storage mode.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// These variables store the dimensions of the texture before they are modified.
	int actualTextureWidth = imageWidth, actualTextureHeight = imageHeight;

	// This loads the mipmaps.
	for (unsigned int level = 0; level < mipMapAmount && (imageWidth || imageHeight); level++) {
		unsigned int size = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, imageWidth, imageHeight, 0, size, buffer + offset);
		offset += size;
		imageWidth /= 2;
		imageHeight /= 2;

		// If these if statements are triggered, the textures do not have power of two dimensions. Eww.
		if (imageWidth < 1) {
			imageWidth = 1;
		}
		if (imageHeight < 1) {
			imageHeight = 1;
		}

	}
	free(buffer);

	GLTexture texture(modelTextureID);
	texture.setDimensions(0, 0, actualTextureWidth, actualTextureHeight);
	texture.setOutput(0, 0, actualTextureWidth, actualTextureHeight);
	return texture;
}*/

// This is a function that uses the C way of loading files to load
// a BMP to a GLuint.
GLuint loadBMPToGLuint(std::string path) {
	// These are variables for the types of data that are read from the file.
	unsigned char header[54];
	unsigned int dataPosition;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char* data;

	// This opens the file with the "read - binary" mode.
	FILE* file;
	fopen_safe(&file, path.c_str(), "rb");
	if (!file) { 
		AelaErrorHandling::consoleWindowError("Project Aela's BMP loader", path + " could not be found.");
		return 0;
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
		AelaErrorHandling::consoleWindowError("Project Aela's BMP loader", path + " does not contain a proper bits per pixel value or is corrupt!");
		return 0;
	}

	dataPosition = *(int*)&(header[10]);
	imageSize = *(int*)&(header[34]);
	width = *(int*)&(header[18]);
	height = *(int*)&(header[22]);

	// If the BMP file is not formatted properly, these statements will trigger.

	if (imageSize == 0) {
		// One byte per RGB value (hence the 24 bpp).
		imageSize = width * height * 3;
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
	GLuint modelTextureID;
	glGenTextures(1, &modelTextureID);
	glBindTexture(GL_TEXTURE_2D, modelTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Since the data is saved to the texture, the data array should be deleted!
	delete[] data;

	return modelTextureID;
}

void loadSkybox(Aela::Skybox* skybox, std::string paths[6], int width, int height) {
	GLuint* cubeMapTexture = skybox->getTexture();
	glGenTextures(1, cubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMapTexture);

	for (unsigned int i = 0; i < 6; i++) {
		loadDDSToCubemapSide(cubeMapTexture, i, paths[i]);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
