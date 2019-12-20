#include "Label.h"

using namespace Aela;

Label::Label(std::string text, Font* font, unsigned int size) : colour(1, 1, 1, 1) {
	this->text = text;
	this->font = font;
	this->size = size;
	setup();
}

Label::Label(std::string text, Font* font, unsigned int size, ColourRGBA* colour) : Label(text, font, size) {
	this->colour = *colour;
}

Label::Label(std::string text, Font *font, unsigned int size, PositioningMode2D positioningMode) : Label(text, font, size) {
	this->positioningMode = positioningMode;
}


Label::Label(std::string text, Font* font, unsigned int size, ColourRGBA* colour, PositioningMode2D positioningMode) : Label(text, font, size, colour) {
	this->positioningMode = positioningMode;
}


Label::~Label() {
}

void Label::setup() {
	setupWidthAndHeight();
}

void Label::updateComponent() {
}

void Label::renderComponent(GLRenderer& renderer) {
	if (font != nullptr) {
		ColourRGBA textColour(colour.getVec4() * tint.getVec4());
		// renderer.renderText(text, font, size, &dimensions, &textColour, positioningMode);
		renderer.renderLabel(this, positioningMode);
	}
}

void Label::renderWithDifferentTint(GLRenderer& renderer, ColourRGBA* tint) {
	if (font != nullptr) {
		ColourRGBA textColour(colour.getVec4() * tint->getVec4());
		renderer.renderText(text, font, size, &dimensions, &textColour, positioningMode);
	}
}

void Label::useFontSize() {
	size = font->getDefaultSize();
}

void Aela::Label::setBuffer(std::vector<unsigned char>* buffer) {
	this->buffer = *buffer;
}

std::vector<unsigned char>* Aela::Label::getBuffer() {
	return &buffer;
}

void Aela::Label::setBufferWidth(unsigned int bufferWidth) {
	this->bufferWidth = bufferWidth;
}

unsigned int Aela::Label::getBufferWidth() {
	return bufferWidth;
}

void Aela::Label::setBufferHeight(unsigned int bufferHeight) {
	this->bufferHeight = bufferHeight;
}

unsigned int Aela::Label::getBufferHeight() {
	return bufferHeight;
}

void Aela::Label::setNeedToReCreateBuffer(bool needToRecreateBuffer) {
	this->needToRecreateBuffer = needToRecreateBuffer;
}

bool Aela::Label::getNeedToRecreateBuffer() {
	return needToRecreateBuffer;
}

void Aela::Label::setCharacterPositioning(Rect<float>* characterPositioning) {
	this->characterPositioning = *characterPositioning;
}

Rect<float>* Aela::Label::getCharacterPositioning() {
	return &characterPositioning;
}

void Label::setText(std::string text) {
	this->text = std::move(text);
	setup();
	markDirty();
	needToRecreateBuffer = true;
}

std::string& Label::getText() {
	return text;
}

void Label::setFont(Font* font) {
	this->font = font;
	setupWidthAndHeight();
	markDirty();
	needToRecreateBuffer = true;
}

Font* Label::getFont() {
	return font;
}

void Aela::Label::setSize(unsigned int size) {
	this->size = size;
	needToRecreateBuffer = true;
}

unsigned int Aela::Label::getSize() {
	return size;
}

void Label::setColour(ColourRGBA* colour) {
	this->colour = *colour;
	markDirty();
	needToRecreateBuffer = true;
}

ColourRGBA* Label::getColour() {
	return &colour;
}

void Label::setupWidthAndHeight() {
	if (font != nullptr) {
		Rect<int> newDimensions = font->getDimensionsOfText(text, size);
		dimensions.setDimensions(newDimensions.getWidth(), newDimensions.getHeight());
	}
}
