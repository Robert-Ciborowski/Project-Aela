#include "GLSpriteSheetLoader.h"
#include "../../Error Handler/ErrorHandling.h"

Aela::GLSpriteSheetLoader::GLSpriteSheetLoader() = default;

Aela::GLSpriteSheetLoader::~GLSpriteSheetLoader() = default;

bool Aela::GLSpriteSheetLoader::load(ResourceMap& resources, std::string src) {
	std::string format = src.substr(src.size() - 4, 4);

	// Note: I've seen some programs output image files as ".PNG" instead of ".png". Why would you do that? No clue.
	// Don't believe me? Try using Windows' Snipping Tool.
	if (format == ".png" || format == ".PNG") {
		if (!loadPNGSpriteSheet(resources, src)) {
			AelaErrorHandling::consoleWindowError("Aela Sprite Sheet Loader", "The file " + src + " could not load.");
			return false;
		}
	} else {
		AelaErrorHandling::consoleWindowError("Aela Sprite Sheet Loader", "The file " + src + " uses a format that is" +
		" not currently supported by the Sprite Sheet Loader.");
	}

	return true;
}

void Aela::GLSpriteSheetLoader::setSpriteWidth(unsigned int spriteWidth) {
	this->spriteWidth = spriteWidth;
}

void Aela::GLSpriteSheetLoader::setSpriteHeight(unsigned int spriteHeight) {
	this->spriteHeight = spriteHeight;
}

bool Aela::GLSpriteSheetLoader::loadPNGSpriteSheet(ResourceMap& resources, std::string src) {
	png_bytep* rows = nullptr;
	png_byte** buffers;
	unsigned int imageWidth = 0, imageHeight = 0;
	unsigned int bytesPerPixel = 0;

	if (!loadPNGToBuffer(src, &rows, &imageWidth, &imageHeight, &bytesPerPixel)) {
		return false;
	}

	if (imageWidth % spriteWidth != 0 || imageHeight % spriteHeight != 0) {
		AelaErrorHandling::consoleWindowError("Aela Sprite Sheet Loader", "The sprite sheet " + src
		+ " cannot properly fit every sprite without cutting one or more sprite off! Sprite Width: "
		+ std::to_string(spriteWidth) + ", Sprite Height: " + std::to_string(spriteHeight));
		delete[] rows;
		return false;
	}

	unsigned int numberOfSpritesAcross = imageWidth / spriteWidth;
	unsigned int numberOfSpritesDown = imageHeight / spriteHeight;

	unsigned int numberOfBuffers = numberOfSpritesAcross * numberOfSpritesDown;
	unsigned int bufferSize = imageWidth * imageHeight * bytesPerPixel;
	buffers = new png_byte*[numberOfBuffers];

	for (unsigned int i = 0; i < numberOfBuffers; i++) {
		buffers[i] = new png_byte[bufferSize];
	}

	for (unsigned int y = 0; y < imageHeight; y++) {
		png_byte* row = rows[y];
		for (unsigned int x = 0; x < imageWidth; x++) {
			png_byte* ptr = &(row[x * bytesPerPixel]);
			unsigned int bufferX = x / spriteWidth, bufferY = y / spriteHeight;
			unsigned int whichBuffer = bufferY * numberOfSpritesAcross + bufferX;
			unsigned int xInsideOfBuffer = x % spriteWidth, yInsideOfBuffer = y % spriteHeight;
			unsigned int indexInsideOfBuffer = (yInsideOfBuffer * spriteWidth + xInsideOfBuffer) * bytesPerPixel;

			for (unsigned int i = 0; i < bytesPerPixel; i++) {
				buffers[whichBuffer][indexInsideOfBuffer + i] = ptr[i];
			}
		}
	}

	// This sets up the OpenGL textures.
	for (unsigned int y = 0; y < numberOfSpritesDown; y++) {
		for (unsigned int x = 0; x < numberOfSpritesAcross; x++) {
			// This creates an OpenGL texture.
			GLuint id;
			glGenTextures(1, &id);

			// This tells OpenGL that future functions will reference this texture.
			glBindTexture(GL_TEXTURE_2D, id);

			if (bytesPerPixel == 3) {
				// The bytes per pixel is 3!
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spriteWidth, spriteHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buffers[y * numberOfSpritesAcross + x]);
			} else {
				// The bytes per pixel is 4!
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spriteWidth, spriteWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffers[y * numberOfSpritesAcross + x]);
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// This selects a filtering to use for the texture. Maybe we should add a feature in which the user
			// of the engine can select a type of filtering to use.
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glGenerateMipmap(GL_TEXTURE_2D);

			std::string resourceID = src.substr(0, src.size() - 4) + "/" + std::to_string(x) + "/" + std::to_string(y)
				+ src.substr(src.size() - 4, 4);
			GLTexture* texture = new GLTexture(resourceID, id);
			texture->setDimensions(0, 0, spriteWidth, spriteHeight);
			resources.put(resourceID, texture);
		}
	}

	for (unsigned int y = 0; y < imageHeight; y++) {
		delete rows[y];
	}
	delete[] rows;

	for (unsigned int i = 0; i < numberOfBuffers; i++) {
		delete[] buffers[i];
	}
	delete[] buffers;

	return true;
}
