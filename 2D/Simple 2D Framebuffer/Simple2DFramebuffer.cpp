/*
* Class: Simple 2D Framebuffer
* Author : Robert Ciborowski
* Date : 04 / 07 / 2017
* Description : A class used by Aela's 2D GLRenderer to represent a 2D frame buffer.
*/

#include "Simple2DFramebuffer.h"

using namespace Aela;

unsigned int* Simple2DFramebuffer::getFramebuffer() {
	return &framebuffer;
}

void Simple2DFramebuffer::setFramebuffer(unsigned int* framebuffer) {
	this->framebuffer = *framebuffer;
}

unsigned int* Simple2DFramebuffer::getMultisampledFramebuffer() {
	return &multisampledFramebuffer;
}

void Simple2DFramebuffer::setMultisampledFramebuffer(unsigned int* framebuffer) {
	multisampledFramebuffer = *framebuffer;
}

unsigned int Simple2DFramebuffer::getMultisampling() {
	return multisampling;
}

void Simple2DFramebuffer::setMultisampling(unsigned int multisampling) {
	this->multisampling = multisampling;
}

void Aela::Simple2DFramebuffer::useRendererMultisamplingLevel(bool usingRendererMultisamplingLevel) {
	this->usingRendererMultisamplingLevel = usingRendererMultisamplingLevel;
}

bool Aela::Simple2DFramebuffer::isUsingRendererMultsamplingLevel() {
	return usingRendererMultisamplingLevel;
}
