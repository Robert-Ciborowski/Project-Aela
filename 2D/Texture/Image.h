/*
* Class: Image
* Author: Robert Ciborowski, adapted from GLTexture (Robert Ciborowski)
* Date: 21/02/2017
* Description: A class used by Aela's Renderer to store properties of an OpenGL texture.
*              Note: This class contains calls to a pure virtual function. This means that
*              only subclasses of this class can be used as objects.
*/

#pragma once
#include "../../Utilities/Rect/Rect.h"

namespace Aela {
	class Image {
		public:
			Image() {
				dimensions = Rect<int>(0, 0, 0, 0);
			}

			virtual ~Image() {
				// Remember to delete texture in your subclass's destructor!
			}

			// These are some getters and setters.
			void setDimensions(Rect<int>* dimensions) {
				this->dimensions = *dimensions;
			}

			void setDimensions(int x, int y, int width, int height) {
				dimensions = Rect<int>(x, y, width, height);
			}

			Rect<int>* getDimensions() {
				return &dimensions;
			}

			unsigned int* getTexture() {
				return &texture;
			}

			// This function returns if the object was initialised properly. If it is not
			// initialised then rendering propblems will occur. Note that a GLTexture is still
			// initialised if the output (dimensions on the screen) has a width and/or height
			// of zero.
			bool isInitialised() {
				return (dimensions.getWidth() > 0 && dimensions.getHeight() > 0 && texture != 0);
		}

		protected:
			Rect<int> dimensions;
			unsigned int texture = 0;
	};
}