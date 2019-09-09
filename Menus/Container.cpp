/*
* Class: Container
* Author: Robert Ciborowski
* Date: 20/03/2017
* Description: Represents a container in our menu system. It lives inside another container,
*              and can contain various containers, widgets and other assorted GUI elements.
*/

#include "Container.h"
#include "AbstractLayoutManager.h"

using namespace Aela;

Container::Container() {
	layout = new AbstractLayoutManager(this);
}

Container::Container(int x, int y) : Component(x, y) {
	layout = new AbstractLayoutManager(this);
}

Container::~Container() {
	for (auto child : children) {
		child.reset();
	}
	delete layout;
}

bool Container::remove(std::shared_ptr<Component> component) {
	// Normally, I would use an id system so that the user could call remove(id),
	// but I don't want to modify someone else's code too much, so I made this for now.
	// - Robert

	auto iter = std::find(children.begin(), children.end(), component);

	if (iter != children.end()) {
		children.erase(iter);
		markDirty();
		return true;
	}
	return false;
}

void Container::clear() {
	children.clear();
	markDirty();
}

void Container::add(std::shared_ptr<Component> component) {
	component->setParent(this);
	component.get()->onAdd(std::bind(&Component::markDirty, this));
	children.push_back(component);
}

void Aela::Container::markDirty() {
	if (!dirty) {
		dirty = true;

		if (dirtyCallback != nullptr) {
			dirtyCallback();
		}

		for (auto& child : children) {

			// child->markDirty();

		}
	}
}

void Container::updateComponent() {
	for (auto child : children) {
		child->update();
	}

	layout->update(children);
}

void Container::renderComponent(GLRenderer& renderer) {
	for (auto& child : children) {
		child->render(renderer);
	}
}

void Container::onMousePressed(MouseEvent* event) {
	int x = event->getMouseXInRendererOutput();
	int y = event->getMouseYInRendererOutput();

	for (auto i = children.rbegin(); i != children.rend(); ++i) {
		auto child = *i;
		child->handleMousePressed(event);
	}
}

void Container::onMouseReleased(MouseEvent* event) {
	int x = event->getMouseXInRendererOutput();
	int y = event->getMouseYInRendererOutput();

	for (auto i = children.rbegin(); i != children.rend(); ++i) {
		auto child = *i;
		child->handleMouseReleased(event);
	}
}

void Container::onMouseMoved(MouseEvent* event) {
	int x = event->getMouseXInRendererOutput();
	int y = event->getMouseYInRendererOutput();

	for (auto i = children.rbegin(); i != children.rend(); ++i) {
		auto child = *i;
		child->handleMouseMoved(event);
	}
}