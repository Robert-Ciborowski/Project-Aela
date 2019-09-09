/*
* Class: GLTexture
* Author: Robert Ciborowski
* Date: 21/02/2017
* Description: A class used by Aela's GLRenderer to store properties of an OpenGL texture.
*/

#pragma once
#include "../../Utilities/Rect/Rect.h"
#include "../../Resource Management/Resource.h"
#include "Texture.h"
#include "GLImage.h"

namespace Aela {
	class GLTexture : public Texture {
		public:
			GLTexture(std::string src, unsigned int& texId) : Texture(src, texId) {
				image.setTexture(texId);
			}

			virtual ~GLTexture() {}

			// These are some getters and setters.
			void setDimensions(Rect<int>* dimensions) {
				image.setDimensions(dimensions);
			}

			void setDimensions(int x, int y, int width, int height) {
				image.setDimensions(x, y, width, height);
			}

			void setTexture(unsigned int texture) {
				image.setTexture(texture);
			}

			Rect<int>* getDimensions() {
				return image.getDimensions();
			}

			Image* getImage() {
				return &image;
			}

			unsigned int* getTexture() {
				return image.getTexture();
			}

			// This function returns if the object was initialised properly. If it is not
			// initialised then rendering propblems will occur. Note that a GLTexture is still
			// initialised if the output (dimensions on the screen) has a width and/or height
			// of zero.
			bool isInitialised() {
				return image.isInitialised();
			}

		private:
			GLImage image;
	};
}