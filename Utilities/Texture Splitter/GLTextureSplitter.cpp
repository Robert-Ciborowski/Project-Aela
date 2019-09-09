/*
* Class: OpenGL Texture Splitter
* Author: Robert Ciborowski
* Date: 03/12/2017
* Description: A utility class generates multiple new textures by splitting an existing one.
*/

#include "GLTextureSplitter.h"

bool Aela::GLTextureSplitter::split(GLTexture* texture, unsigned int newTextureWidth,
	unsigned int newTextureHeight, std::vector<GLTexture>* locationOfNewTextures) {
	unsigned short* data = nullptr;
	glBindTexture(GL_TEXTURE_2D, *texture->getTexture());
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_SHORT, data);

	if (data == nullptr) {
		return false;
	}

	std::vector<unsigned short> dataOfNewImages;
	locationOfNewTextures->clear();

	for (unsigned int whichSection = 0; whichSection < (texture->getDimensions()->getWidth() / newTextureWidth)
		* (texture->getDimensions()->getHeight() / newTextureHeight); whichSection++) {
		for (unsigned int y = 0; y < newTextureHeight; y++) {
			for (unsigned int x = 0; x < newTextureWidth; x++) {
				unsigned int whichIndex = (whichSection * newTextureWidth * newTextureHeight + (newTextureHeight * y + x)) * 4;
				for (unsigned int j = 0; j < 4; j++) {
					dataOfNewImages.push_back(data[whichIndex + j]);
				}
				// locationOfNewTextures->push_back(GLTexture());


				dataOfNewImages.clear();
			}
		}
	}

	return true;
}