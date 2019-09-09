/*
* Class: Rectangle
* Author: Robert Ciborowski
* Date: 04/02/2017
* Description: A simple header-only class for rectangles.
*/

#pragma once
#include "../strut.h"

namespace Aela {
	template <class T> class Rect {
		public:
			Rect() {
				xPosition = 0;
				yPosition = 0;
				width = 0;
				height = 0;
			}

			Rect(T xPosition, T yPosition, T width, T height) {
				this->xPosition = xPosition;
				this->yPosition = yPosition;
				this->width = width;
				this->height = height;
			}

			void setX(T xPosition) {
				this->xPosition = xPosition;
			}

			void setY(T yPosition) {
				this->yPosition = yPosition;
			}

			void setXY(T xPosition, T yPosition) {
				this->xPosition = xPosition;
				this->yPosition = yPosition;
			}

			// Should this be called "setWidthHeight" or "setWidthAndHeight"?
			void setWidthHeight(T width, T height) {
				this->width = width;
				this->height = height;
			}

			void setWidth(T width) {
				this->width = width;
			}

			void setHeight(T height) {
				this->height = height;
			}

			void setDimensions(T width, T height) {
				this->width = width;
				this->height = height;
			}

			void getValues(T* xPosition, T* yPosition, T* width, T* height) {
				*xPosition = this->xPosition;
				*yPosition = this->yPosition;
				*width = this->width;
				*height = this->height;
			}

			T getX() {
				return xPosition;
			}

			T getY() {
				return yPosition;
			}

			T getWidth() {
				return width;
			}

			T getHeight() {
				return height;
			}

			bool contains(T x, T y) {
				return x >= xPosition && x < xPosition + width && y >= yPosition
					&& y < yPosition + height;
			}

			// This returns a string with all of the properties of the transformable.
			virtual std::string getPropertiesAsString(int numberOfTrailingZeroes) {
				std::string s = "";
				s += "Position: " + toStringWithATrailingZero((float) xPosition) + " " + toStringWithATrailingZero((float) yPosition)
					+ ", ";
				s += "Width: " + toStringWithATrailingZero((float) width) + ", ";
				s += "Height: " + toStringWithATrailingZero((float) height);
				return s;
			}

			inline friend std::ostream& operator<<(std::ostream &os, Rect<T> const& me) {
				return (os << "Rect(" << me.xPosition << ", " << me.yPosition << ", " << me.width << ", " << me.height << ")");
			}

			Rect<T> operator+(Rect<T> const& other) {
				return Rect(xPosition + other.xPosition, yPosition + other.yPosition, width + other.width, height + other.height);
			}

			Rect<T> operator-(Rect<T> const& other) {
				return Rect(xPosition - other.xPosition, yPosition - other.yPosition, width - other.width, height - other.height);
			}

			void operator+=(Rect<T> other) {
				*this = *this + other;
			}

			void operator-=(Rect<T> other) {
				*this = *this - other;
			}

		protected:
			T xPosition, yPosition, width, height;
	};
}