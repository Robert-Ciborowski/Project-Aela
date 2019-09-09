/*
* Class: Simple 2D Framebuffer
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used by Aela's 2D Renderer to represent a 2D frame buffer.
*/

#pragma once

#include "../../2D/Texture/GLImage.h"
#include "Simple2DFramebuffer.h"

namespace Aela {
	class Simple2DGLFramebuffer : public Simple2DFramebuffer {
		public:
			Simple2DGLFramebuffer() {

			}

			Image* getFramebufferImage();
			Image* getMultisampledFramebufferImage();

		private:
			GLImage framebufferTexture, multisampledFramebufferTexture;
	};
}