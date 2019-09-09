#include "Component.h"

#include "../Events/EventConstants.h"

using namespace Aela;

Component::Component() {}

Component::Component(int x, int y) {
	dimensions = Rect<int>(x, y, 0, 0);
}

Component::Component(int x, int y, int width, int height) {
	dimensions = Rect<int>(x, y, width, height);
}

Component::~Component() {
}

void Component::update() {
	updateComponent();

	// Delete this line and uncomment the block below to return to the old, WIP menu system!
	dirty = true;

	/*if (dirty) {
		dirtyOnUpdate = true;
	}*/
}

void Component::render(GLRenderer& renderer) {
	// Delete this block and uncomment the block below to return to the old, WIP menu system!
	if (dirty && visible) {
		renderComponent(renderer);
		dirty = false;
	}

	/*if (dirtyOnUpdate && visible) {
		renderComponent(renderer);
		dirty = false;
		dirtyOnUpdate = false;
	}*/
}

void Component::handleMousePressed(MouseEvent* event) {
	if (event->getType() != EventConstants::MOUSE_PRESSED) {
		return;
	}

	if (alwaysTriggerMouseEvents || dimensions.contains(event->getMouseXInRendererOutput(), event->getMouseYInRendererOutput())) {
		onMousePressed(event);
		listeners.fire(event);

		event->consume();
	}
}

void Component::handleMouseReleased(MouseEvent* event) {
	if (event->getType() != EventConstants::MOUSE_RELEASED) {
		return;
	}

	if (alwaysTriggerMouseEvents || dimensions.contains(event->getMouseXInRendererOutput(), event->getMouseYInRendererOutput())) {
		onMouseReleased(event);
		listeners.fire(event);

		event->consume();
	}
}

void Component::handleMouseMoved(MouseEvent* event) {
	if (event->getType() != EventConstants::MOUSE_MOTIONED) {
		return;
	}

	onMouseMoved(event);

	bool contains = alwaysTriggerMouseEvents || dimensions.contains(event->getMouseXInRendererOutput(), event->getMouseYInRendererOutput());

	if (contains) {
		if (!hovered) {
			hovered = true;
			// dirty = true;

			onMouseEntered(event);
			listeners.fire(EventConstants::MOUSE_ENTERED_COMPONENT, event);
		}
	} else {
		if (hovered) {
			hovered = false;
			// dirty = true;

			onMouseExited(event);
			listeners.fire(EventConstants::MOUSE_EXITED_COMPONENT, event);
		}
	}
}

void Component::onMousePressed(MouseEvent* event) {}
void Component::onMouseReleased(MouseEvent* event) {}
void Component::onMouseMoved(MouseEvent* event) {}
void Component::onMouseEntered(MouseEvent* event) {}
void Component::onMouseExited(MouseEvent* event) {}

void Component::addListener(int type, EventListener listener) {
	listeners.put(type, listener);
}

void Component::markDirty() {
	if (!dirty) {
		dirty = true;

		if (parent != nullptr) {
			parent->markDirty();
		}

		if (dirtyCallback != nullptr) {
			dirtyCallback();
		}
	}
}

void Aela::Component::setParent(Component* parent) {
	this->parent = parent;
}

void Aela::Component::show() {
	visible = true;
	markDirty();
}

void Aela::Component::hide() {
	visible = false;
	markDirty();
}

void Aela::Component::toggleVisibility() {
	visible = !visible;
	markDirty();
}

bool Aela::Component::isVisible() {
	return visible;
}

bool Component::isDirty() {
	return dirty;
}

void Component::onAdd(std::function<void()> dirtyCallback) {
	this->dirtyCallback = dirtyCallback;
}
