/*
* Class: RGBA Colour
* Author: Robert Ciborowski
* Date: March 15th, 2017
* Description: A simple header-only class for RGBA colour.
*/

#pragma once
#include <glm/glm.hpp>
#include <ostream>
#include <iostream>
#include <algorithm>

namespace Aela {
	class ColourRGBA {
		public:
			ColourRGBA() {
				set(0, 0, 0, 0);
			}

			bool operator==(const ColourRGBA& other) {
				return r == other.r && b == other.b && g == other.g && a == other.a;
			}
			
			bool operator!=(const ColourRGBA& other) {
				return !(*this == other);
			}

			ColourRGBA operator*(const ColourRGBA& other) {
				return ColourRGBA(std::min(1.0f, r * other.r), std::min(1.0f, g * other.g),
					std::min(1.0f, b * other.b), std::min(1.0f, a * other.a));
			}

			ColourRGBA operator/(const ColourRGBA& other) {
				return ColourRGBA(std::min(1.0f, r / other.r), std::min(1.0f, g / other.g),
					std::min(1.0f, b / other.b), std::min(1.0f, a / other.a));
			}

			ColourRGBA(glm::vec3* colour, float alpha) {
				set(colour->r, colour->g, colour->b, alpha);
			}

			ColourRGBA(glm::vec4* colour) {
				set(colour->r, colour->g, colour->b, colour->a);
			}

			ColourRGBA(float r, float g, float b, float a) {
				set(r, g, b, a);
			}

			ColourRGBA(float value) {
				set(value, value, value, value);
			}

			ColourRGBA(glm::vec4 vec4) {
				set(vec4.r, vec4.g, vec4.b, vec4.a);
			}

			void setR(float r) {
				this->r = r;
			}

			void setG(float g) {
				this->g = g;
			}

			void setB(float b) {
				this->b = b;
			}

			void setA(float a) {
				this->a = a;
			}

			void set(float r, float g, float b, float a) {
				this->r = r;
				this->g = g;
				this->b = b;
				this->a = a;
			}

			void set(ColourRGBA& other) {
				this->r = other.r;
				this->g = other.g;
				this->b = other.b;
				this->a = other.a;
			}

			float getR() {
				return r;
			}

			float getG() {
				return g;
			}

			float getB() {
				return b;
			}

			float getA() {
				return a;
			}

			glm::vec4 getVec4() {
				return glm::vec4(r, g, b, a);
			}

			inline friend std::ostream& operator<<(std::ostream &os, ColourRGBA const& me) {
				return (os << "ColourRGBA(" << me.r << ", " << me.g << ", " << me.b << ", " << me.a << ")");
			}

		private:
			float r, g, b, a;
	};
}