/*
* Class: Font
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used by Aela's 2D GLRenderer to store text data.
*/

#include "Font.h"
#include "../../Error Handler/ErrorHandling.h"
#include <iostream>
#include <signal.h>

using namespace Aela;

Font::Font(std::string src, FT_Face face) : Resource(src) {
	this->face = face;
}

FT_Face Font::getFace() {
	return face;
}

void Font::setDefaultSize(unsigned int defaultSize) {
	if (defaultSize != this->defaultSize) {
		this->defaultSize = defaultSize;
		if (FT_Set_Pixel_Sizes(face, 0, defaultSize * superSamplingFactor) != 0) {
			AelaErrorHandling::consoleWindowError("Font", "Error with resizing a font.");
		}

		//FT_Set_Char_Size(
		//	face,    /* handle to face object           */
		//	0,       /* char_width in 1/64th of points  */
		//	size * 64,   /* char_height in 1/64th of points */
		//	100);   /* vertical device resolution      */
	}
}

unsigned int Font::getDefaultSize() {
	return defaultSize;
}

void Aela::Font::prepareForRendering(unsigned int superSamplingFactor, unsigned int size) {
	if (this->superSamplingFactor != superSamplingFactor || fixSizeNextRender || size != this->renderingSize) {
		this->superSamplingFactor = superSamplingFactor;
		this->renderingSize = size;
		if (FT_Set_Pixel_Sizes(face, 0, size * superSamplingFactor) != 0) {
			AelaErrorHandling::consoleWindowError("Font", "Error with resizing a font.");
		}
	}
}

Rect<int> Aela::Font::getDimensionsOfText(std::string text) {
	return getDimensionsOfText(text, defaultSize);
}

Rect<int> Font::getDimensionsOfText(std::string text, unsigned size) {
	// If the font just came back from rendering, it should recall FT_Set_Pixel_Size.
	if (superSamplingFactor != 1 || this->renderingSize != size || fixSizeNextRender) {
		if (FT_Set_Pixel_Sizes(face, 0, size) != 0) {
			AelaErrorHandling::consoleWindowError("Font", "Error with resizing a font.");
		}
		this->superSamplingFactor = 1;
		fixSizeNextRender = true;
	}

	FT_BBox bbox = face->bbox;
	FT_Error error = 0;
	Rect<int> dimensions;

	// I would not normally use this approach to calculate width because it is a little faster to use FT_Glyph_Get_CBox.
	// However, that function causes an access violation and this method is slightly more accurate.
	// Update: Actually, calculating width with CBox may not make sense. I will investigate further.
	double width = 0;
	for (unsigned int i = 0; i < text.size(); i++) {
		// AelaErrorHandling::handleSignal(SIGSEGV);

		// FT_Load_Char was causing memory access violations. After using AelaErrorHandling's signal.h-related
		// features, I managed to catch the violation as an exception (to prevent crashing) and also found that
		// FT_Load_Char gives various errors.
		try {
			error = FT_Load_Char(face, static_cast<FT_ULong>(text[i]), FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT);
			if (error) {
				AelaErrorHandling::consoleWindowError("Aela Text Rendering", "FreeType Error: " + (std::string) AelaErrorHandling::getFreeTypeErrorMessage(error));
				continue;
			}
		} catch (char* e) {
			std::cout << text << " " << face->family_name << face << " " << (char) (text.at(i)) << " " << AelaErrorHandling::getFreeTypeErrorMessage(error) << "\n";
			std::cout << e << " is an FT_Load_Char exception\n";
			continue;
		}

		FT_GlyphSlot glyph = face->glyph;
		// width += glyph->bitmap.width;
		width += (float) glyph->bitmap.width;
		unsigned int horiAdvance = glyph->metrics.horiAdvance / POINTS_PER_PIXEL;
		unsigned int advancement = horiAdvance - glm::clamp(glyph->bitmap.width, (unsigned int) 0, horiAdvance);
		width += advancement;
	}

	// This sets the dimensions object of the text to what it should be.
	dimensions.setWidth((int) width);
	int height = (bbox.yMax - bbox.yMin) / POINTS_PER_PIXEL;
	dimensions.setHeight(height);
	return dimensions;
}

void Font::setAntialiasing(bool antialiasing) {
	this->antialiasing = antialiasing;
}

bool Font::isAntialiased() {
	return antialiasing;
}

void Font::setFace(FT_Face face) {
	this->face = face;
}
