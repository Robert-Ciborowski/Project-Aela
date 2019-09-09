/*
* Class: FontLoader
* Author: Robert Ciborowski
* Date: 24/08/2018
* Description: A resource loader which loads fonts.
*/

#pragma once
#include "../../Resource Management/ResourceLoader.h"
#include <ft2build.h>
#include <freetype/freetype.h>

namespace Aela {
	class FontLoader : public ResourceLoader {
		public:
			FontLoader();

			bool load(ResourceMap& resources, std::string src) override;

		private:
			// This is the FreeType Library object.
			static FT_Library freetype;
	};
}