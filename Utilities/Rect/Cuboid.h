/*
* Class: Cuboid
* Author: Robert Ciborowski
* Date: 05/08/2017
* Description: A simple header-only class for cuboids (rectangular prisms).
*              Note: I made this by pretty much just copying and pasting stuff from Rect.
*/

#pragma once

#include "Rect.h"

namespace Aela {
	template <class T> class Cuboid : public Rect<T> {
		public:
			Cuboid() {
				this->xPosition = 0;
				this->yPosition = 0;
				this->zPosition = 0;
				this->width = 0;
				this->height = 0;
				this->depth = 0;
			}

			Cuboid(T xPosition, T yPosition, T zPosition, T width, T height, T depth) {
				this->xPosition = xPosition;
				this->yPosition = yPosition;
				this->zPosition = zPosition;
				this->width = width;
				this->height = height;
				this->depth = depth;
			}

			void setZ(T zPosition) {
				this->zPosition = zPosition;
			}

			void setXYZ(T xPosition, T yPosition, T zPosition) {
				this->xPosition = xPosition;
				this->yPosition = yPosition;
				this->zPosition = zPosition;
			}

			void setDepth(T depth) {
				this->depth = depth;
			}

			T getZ() {
				return zPosition;
			}

			T getDepth() {
				return depth;
			}

			void setWidthHeightDepth(T width, T height, T depth) {
				this->width = width;
				this->height = height;
				this->depth = depth;
			}

			void getValues(T* xPosition, T* yPosition, T* zPosition, T* width, T* height, T* depth) {
				*xPosition = this->xPosition;
				*yPosition = this->yPosition;
				*zPosition = this->zPosition;
				*width = this->width;
				*height = this->height;
				*depth = this->depth;
			}

			// This returns a string with all of the properties of the transformable.
			std::string getPropertiesAsString() {
				std::string s = "";
				s += "Position: " + toStringWithATrailingZero((float) this->xPosition) + " " + toStringWithATrailingZero((float) this->yPosition)
					+ " " + toStringWithATrailingZero((float) this->zPosition) + ", ";
				s += "Width: " + toStringWithATrailingZero((float) this->width) + ", ";
				s += "Height: " + toStringWithATrailingZero((float) this->height) + ", ";
				s += "Depth: " + toStringWithATrailingZero((float) this->depth);
				return s;
			}

			inline friend std::ostream& operator<<(std::ostream &os, Cuboid<T> const& me) {
				return (os << "Rect(" << me.xPosition << ", " << me.yPosition << ", " << me.zPosition << ", " << me.width << ", "
					<< me.height << ", " << me.getDepth << ")");
			}

		protected:
			T zPosition, depth;
	};
}