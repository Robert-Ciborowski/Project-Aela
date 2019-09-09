/*
* Class: Simple 2D Framebuffer
* Author: Robert Ciborowski
* Date: 04/07/2017
* Description: A class used by Aela's 2D Renderer to represent a 2D frame buffer.
*/

#pragma once
#include "../Texture/Image.h"

namespace Aela {
	class Simple2DFramebuffer {
		public:
			Simple2DFramebuffer() {

			}

			~Simple2DFramebuffer() {

			}

			unsigned int* getFramebuffer();
			void setFramebuffer(unsigned int* framebuffer);
			unsigned int* getMultisampledFramebuffer();
			void setMultisampledFramebuffer(unsigned int* framebuffer);
			virtual Image* getFramebufferImage() = 0;
			virtual Image* getMultisampledFramebufferImage() = 0;
			unsigned int getMultisampling();
			void setMultisampling(unsigned int multisampling);
			void useRendererMultisamplingLevel(bool use);
			bool isUsingRendererMultsamplingLevel();

		protected:
			unsigned int framebuffer, multisampledFramebuffer;
			unsigned int multisampling = 0;
			bool usingRendererMultisamplingLevel = true;
	};
}