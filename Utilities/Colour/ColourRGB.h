/*
* Class: RGB Colour
* Author: Robert Ciborowski
* Date: April 26th, 2017
* Description: A simple header-only class for RGB colour.
*/

#pragma once
#include <glm/glm.hpp>
#include <iostream>

namespace Aela {
	class ColourRGB {
		public:
			ColourRGB() {

			}

			ColourRGB(glm::vec3* colour) {
				set(colour->r, colour->g, colour->b);
			}

			ColourRGB(float r, float g, float b) {
				set(r, g, b);
			}

			ColourRGB(float value) {
				set(value, value, value);
			}

			bool operator==(const ColourRGB& other) {
				return r == other.r && b == other.b && g == other.g;
			}

			bool operator!=(const ColourRGB& other) {
				return !(*this == other);
			}

            inline friend std::ostream& operator<<(std::ostream& os, ColourRGB const& me) {
                os << "ColourRGB(" << me.r << ", " << me.g << ", " << me.b << ")";
                return os;
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

			void set(float r, float g, float b) {
				this->r = r;
				this->g = g;
				this->b = b;
			}

			void set(ColourRGB& other) {
				this->r = other.r;
				this->g = other.g;
				this->b = other.b;
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

			float* getRPointer() {
				return &r;
			}

			float* getGPointer() {
				return &g;
			}

			float* getBPointer() {
				return &b;
			}

			glm::vec3 getVec3() {
				return glm::vec3(r, g, b);
			}

			glm::vec3* getVec3Ptr() {
				vec = glm::vec3(r, g, b);
				return &vec;
			}

		private:
			float r, g, b;
			glm::vec3 vec;
	};
}