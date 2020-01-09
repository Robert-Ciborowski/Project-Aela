/*
* Class: Font
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used by Aela's 2D GLRenderer to store text data.
*/

#pragma once
#include "../../Resource Management/Resource.h"
#include "../../Utilities/Rect/Rect.h"
#include <string>
#include <ft2build.h>
#include <freetype/freetype.h>

// This stores the amount of points in a character that make up one pixel.
#define POINTS_PER_PIXEL 64


namespace Aela {
	class Font : public Resource {
		public:
			Font(std::string src, FT_Face face);

			// These are the getters and setters.
			void setFace(FT_Face face);
			FT_Face getFace();
			void setDefaultSize(unsigned int defaultSize);
			unsigned int getDefaultSize();
			void prepareForRendering(unsigned int superSamplingFactor, unsigned int size);
			Rect<int> getDimensionsOfText(std::wstring text);
			Rect<int> getDimensionsOfText(std::wstring text, unsigned int size);
			Rect<int> getDimensionsOfText(std::string text);
			Rect<int> getDimensionsOfText(std::string text, unsigned int size);
			void setAntialiasing(bool antialiasing);
			bool isAntialiased();

		private:
			FT_Face face;
			unsigned int defaultSize = 0;
			unsigned int renderingSize = 0;
			unsigned int superSamplingFactor = 1;
			bool antialiasing = true;
			bool fixSizeNextRender = false;
	};
}
