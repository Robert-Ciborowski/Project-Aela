#include "TextInput.h"

using namespace Aela;

Aela::TextInput::TextInput(std::string text, Font* font, unsigned int size) : colour(1, 1, 1, 1) {
	this->text = text;
	this->font = font;
	this->size = size;
}

Aela::TextInput::TextInput(std::string text, Font* font, unsigned int size, ColourRGBA* colour) {
	this->text = text;
	this->font = font;
	this->size = size;
	this->colour = *colour;
}

TextInput::~TextInput() {
}

void Aela::TextInput::setup() {
	FT_Face face = font->getFace();
	FT_GlyphSlot glyph = face->glyph;
	FT_BBox bbox = face->bbox;

	// determine the height of this textfield
	dimensions.setHeight((int) ((bbox.yMax - bbox.yMin) / POINTS_PER_PIXEL));
}

void TextInput::updateComponent() {
}

void TextInput::renderComponent(GLRenderer& renderer) {
	renderer.renderText(text, font, size, &dimensions, &colour, positioningMode);
}

std::string Aela::TextInput::getText() {
	return text;
}

Font* Aela::TextInput::getFont() {
	return font;
}

ColourRGBA* Aela::TextInput::getColour() {
	return &colour;
}
