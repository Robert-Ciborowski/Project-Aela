/*
* Class: OpenGL Texture Splitter
* Author: Robert Ciborowski
* Date: 03/12/2017
* Description: A utility class which generates multiple new textures by splitting an existing one.
*/

#pragma once
#include <vector>
#include "../../2D/Texture/GLTexture.h"
#include "../Rect/Rect.h"

namespace Aela {
	// Don't use this class, it isn't finished! In fact, I found a way to not need to split textures, so I have no
	// pressure to finish this!
	class GLTextureSplitter {
		public:
			GLTextureSplitter() {}

			bool split(GLTexture* texture, unsigned int newTextureWidth, unsigned int newTextureHeight,
				std::vector<GLTexture>* locationOfNewTextures);
	};

}
