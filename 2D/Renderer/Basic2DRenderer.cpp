/*
* Class: Project Aela's 2D Renderer
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used by Aela's Renderer to render 2D objects.
*/

#include "Basic2DRenderer.h"

using namespace Aela;

Basic2DRenderer::Basic2DRenderer() = default;

Basic2DRenderer::~Basic2DRenderer() = default;

void Basic2DRenderer::setWindow(Window* setWindow) {
	window = setWindow;
	window->getDimensions(&windowWidth, &windowHeight);
}

Window* Basic2DRenderer::getWindow() {
	return window;
}
