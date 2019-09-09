/*
* Class: Image
* Author: Robert Ciborowski, adapted from GLTexture (Robert Ciborowski)
* Date: 21/02/2017
* Description: A class used by Aela's Renderer to store properties of an Open texture.
*/

#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include "Image.h"

namespace Aela {
	class GLImage : public Image {
		public:
			GLImage() {
				dimensions = Rect<int>(0, 0, 0, 0);
			}

			virtual ~GLImage() {
				deleteImage();
			}

			void setTexture(unsigned int texture) {
				if (texture != 0) {
					deleteImage();
				}
				// const GLclampf priority = {(GLclampf) 1.0};
				// glPrioritizeTextures(1, &texture, &priority);
				this->texture = texture;
			}

		private:
			void deleteImage() {
				glDeleteTextures(1, &texture);
			}
	};
}
