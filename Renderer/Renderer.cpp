/*
* Class: Renderer
* Author: Robert Ciborowski
* Date: 2/12/2017
* Description: A class used for representing a renderer, generically.
*/

#include "../Events/EventConstants.h"
#include "Renderer.h"
#include "../Events/WindowResizeEvent.h"

using namespace Aela;

Aela::Renderer::Renderer() {
	tint3D = ColourRGBA(1, 1, 1, 1);

	// Set feature deaults here.
	features[RendererFeature::BILLBOARDS] = false;
	features[RendererFeature::SKYBOX] = false;
	features[RendererFeature::SHADOWS] = false;
	features[RendererFeature::LIGHTS] = false;
}

Aela::Renderer::Renderer(Window* window) : Renderer() {
	this->window = window;
}

Aela::Renderer::Renderer(const Renderer& other) : camera() {
	time = other.time;
	window = other.window;
	eventHandler = other.eventHandler;
	tint3D = other.tint3D;
	outputToWindow = other.outputToWindow;
	features = other.features;
	resolutionWasChangedFlag = { false };
	multisampling3D = other.multisampling3D;
	multisampling2D = other.multisampling2D;
}

void Aela::Renderer::onEvent(Event* event) {
	if (event->getType() == EventConstants::WINDOW_RESIZE) {
		auto resizedEvent = dynamic_cast<WindowResizeEvent*>(event);
		if (resizedEvent->getWindow() == window) {
			// Set this internal flag to true so that the resolution change can be handled later.
			resolutionWasChangedFlag = { true };
		}
	}
}

void Renderer::setWindow(Window* window) {
	this->window = window;
	camera.setWindow(window);
}

void Renderer::setTime(Clock* time) {
	this->time = time;
	camera.setTime(time);
}

void Aela::Renderer::setEventHandler(EventHandler* eventHandler) {
	this->eventHandler = eventHandler;
}

unsigned int Aela::Renderer::getMultisampling3D() {
	return multisampling2D;
}

unsigned int Aela::Renderer::getMultisampling2D() {
	return multisampling2D;
}

void Renderer::setFOV(float value) {
	camera.setFieldOfView(value);
}

void Aela::Renderer::set3DTint(ColourRGBA* tint3D) {
	this->tint3D = *tint3D;
}

void Aela::Renderer::clear3DTint() {
	tint3D = ColourRGBA(0, 0, 0, 0);
}

void Aela::Renderer::scissor(int x, int y, size_t width, size_t height) {}

void Aela::Renderer::resetScissor() {}

void Renderer::resolutionWasChanged() {
	resolutionWasChangedFlag = true;
}

void Renderer::setOutputToWindow(Rect<int>* outputToWindow) {
	this->outputToWindow = *outputToWindow;
}

Rect<int>* Renderer::getOutputToWindow() {
	return &outputToWindow;
}

void Renderer::outputToEntireWindow() {
    Rect<int>* rect = new Rect<int>(0, 0, window->getWidth(), window->getHeight());
	setOutputToWindow(rect);
    delete rect;
}

Window* Renderer::getWindow() {
	return window;
}

Clock* Renderer::getClock() {
	return time;
}

Camera3D* Renderer::getCamera() {
	return &camera;
}
