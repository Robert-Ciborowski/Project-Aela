/*
* Class: Simple 2D Framebuffer
* Author : Robert Ciborowski
* Date : 04 / 07 / 2017
* Description : A class used by Aela's 2D GLRenderer to represent a 2D frame buffer.
*/

#include "Simple2DGLFramebuffer.h"

using namespace Aela;

Image* Simple2DGLFramebuffer::getFramebufferImage() {
	return &framebufferTexture;
}

Image* Simple2DGLFramebuffer::getMultisampledFramebufferImage() {
	return &multisampledFramebufferTexture;
}