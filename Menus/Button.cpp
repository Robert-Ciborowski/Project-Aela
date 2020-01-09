#include "Button.h"
#include "../Events/EventConstants.h"

Aela::Button::Button() : hoverTint(0.0f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0),
	textLabel("", nullptr, 0, PositioningMode2D::CENTER) {
}

Aela::Button::Button(GLTexture* texture) : ImageComponent(texture), hoverTint(0.7f, 0.7f, 0.7f, 1.0f),
	clickTint(0.5f, 0.5f, 0.5f, 1.0), textLabel("", nullptr, 0, PositioningMode2D::CENTER) {
}

Aela::Button::Button(GLTexture* texture, Rect<int>* dimensions) : ImageComponent(texture, dimensions),
	hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0), textLabel("", nullptr, 0, PositioningMode2D::CENTER) {
}

Aela::Button::Button(ColourRGBA* hoverTint, ColourRGBA* clickTint) : textLabel("", nullptr, 0, PositioningMode2D::CENTER) {
	this->hoverTint = *hoverTint;
	this->clickTint = *clickTint;
}

Aela::Button::~Button() {
}

void Aela::Button::setFont(Font* font) {
	textLabel.setFont(font);
}

void Aela::Button::setFontSize(unsigned int size) {
	textLabel.setSize(size);
}

void Aela::Button::setTextColour(ColourRGBA* colour) {
	textLabel.setColour(colour);
}

void Aela::Button::updateComponent() {
	if (isDirty()) {
		if (active) {
			tint = clickTint;
		} else if (hovered) {
			tint.set(hoverTint);
		} else {
			tint.set(1, 1, 1, 1);
		}
	}
	
	//std::cout << std::boolalpha << tint << ", ";
	//if (tint == clickTint) {
	//	std::cout << "tint is the click tint";
	//} else if (tint == hoverTint) {
	//	std::cout << "tint is the hover tint";
	//} else {
	//	std::cout << "tint is the normal tint";
	//}
	//std::cout << ", hovered: " << hovered << "\n";

	// TODO figure out why wrong tint is displayed
}

void Aela::Button::renderComponent(GLRenderer& renderer) {
	ImageComponent::renderComponent(renderer);

	textLabel.renderWithDifferentTint(renderer, &tint);
}

void Button::onMousePressed(MouseEvent* event) {
	int x = event->getMouseXInRendererOutput();
	int y = event->getMouseYInRendererOutput();

	if (dimensions.contains(x, y)) {
		clickStarted = true;
		active = true;
		markDirty();
	}
}

void Button::onMouseReleased(MouseEvent* event) {
	if (!clickStarted) {
		return;
	}

	int x = event->getMouseXInRendererOutput();
	int y = event->getMouseYInRendererOutput();

	if (dimensions.contains(x, y)) {
		onClick();
		active = false;
		markDirty();
	}

	clickStarted = false;
}

void Button::onMouseEntered(MouseEvent* event) {
	if (clickStarted) {
		active = true;
	}

	markDirty();
}

void Button::onMouseExited(MouseEvent* event) {
	markDirty();
	active = false;
}

void Aela::Button::setupOnClick(std::function<void()> function) {
	onClick = function;
}

void Aela::Button::setText(std::string text) {
	textLabel.setText(text);
	setupLabelDimensions();
}

void Aela::Button::setText(std::wstring text) {
	textLabel.setText(text);
	setupLabelDimensions();
}

std::wstring Aela::Button::getText() {
	return textLabel.getText();
}

std::string Aela::Button::getTextAsString() {
	return textLabel.getTextAsString();
}

void Aela::Button::setHoverTint(ColourRGBA* hoverTint) {
	this->hoverTint = *hoverTint;
}

void Aela::Button::setClickTint(ColourRGBA* clickTint) {
	this->clickTint = *clickTint;
}

void Aela::Button::setDimensions(Rect<int>* dimensions) {
	this->dimensions = *dimensions;
	setupLabelDimensions();
}

void Aela::Button::setPosition(int x, int y) {
	dimensions.setXY(x, y);
	setupLabelDimensions();
}

void Aela::Button::setWidthAndHeight(int width, int height) {
	dimensions.setWidthHeight(width, height);
	setupLabelDimensions();
}

void Aela::Button::wrapAroundText() {
	Rect<int>* textDimensions = textLabel.getDimensions();
	dimensions.setWidthHeight(textDimensions->getWidth(), textDimensions->getHeight());
	setupLabelDimensions();
}

void Aela::Button::setupLabelDimensions() {
	Rect<int>* textDimensions = textLabel.getDimensions();
	textDimensions->setXY(dimensions.getX() + dimensions.getWidth() / 2,
		dimensions.getY() + dimensions.getHeight() / 2);
}