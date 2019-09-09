/*
* Class: Texture
* Author: Robert Ciborowski
* Date: 21/02/2017
* Description: A class used by Aela's Renderer to store properties of an OpenGL texture.
*/

#pragma once
#include "../../Utilities/Rect/Rect.h"
#include "../../Resource Management/Resource.h"
#include "Image.h"

namespace Aela {
	class Texture : public Aela::Resource {
		public:
			Texture(std::string src, unsigned int& texId) : Resource(src) {}

			virtual ~Texture() {}

			// These are some getters and setters.
			virtual void setDimensions(Rect<int>* dimensions) = 0;

			virtual void setDimensions(int x, int y, int width, int height) = 0;

			virtual void setTexture(unsigned int texture) = 0;

			virtual Rect<int>* getDimensions() = 0;

			virtual Image* getImage() = 0;

			virtual unsigned int* getTexture() = 0;

			// This function returns if the object was initialised properly. If it is not
			// initialised then rendering propblems will occur. Note that a Texture is still
			// initialised if the output (dimensions on the screen) has a width and/or height
			// of zero.
			virtual bool isInitialised() = 0;
	};
}