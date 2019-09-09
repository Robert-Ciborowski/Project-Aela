#include "Menu.h"

using namespace Aela;

Menu::Menu() {
	visible = false;
}

Menu::~Menu() {
}

void Menu::init(Rect<int>* renderDimensions, GLRenderer& renderer) {
	if (!initialized) {
		renderer.setupSimple2DFramebuffer(&buffer, renderDimensions, renderDimensions);
		dimensions = Rect<int>(renderDimensions->getX(), renderDimensions->getY(), renderDimensions->getWidth(), renderDimensions->getHeight());
		initialized = true;
	}
}

void Menu::init(Rect<int>* renderDimensions, GLRenderer& renderer, int x, int y) {
	if (!initialized) {
		renderer.setupSimple2DFramebuffer(&buffer, renderDimensions, renderDimensions);
		dimensions = Rect<int>(x, y, renderDimensions->getWidth(), renderDimensions->getHeight());
		initialized = true;
	}
}

void Menu::render(GLRenderer& renderer) {
	renderer.bindSimple2DFramebuffer(&buffer);

	if (visible && initialized && isDirty()) {
		// If a menu needs to be rerendered, it might be because one of its children decided to become hidden. In that case,
		// the entire buffer of the menu needs to be cleared, followed by the rerendering of all children onto the buffer.
		// TODO check if there is some way to clear a certain part of the buffer
		//      Not sure how old this TODO is, but can't you just use scissoring?
		renderer.clearSimple2DFramebuffer();
		Container::render(renderer);
	}

	renderer.renderSimple2DFramebuffer();
}

bool Menu::isInitialized() {
	return initialized;
}