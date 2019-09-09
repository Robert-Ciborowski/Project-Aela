/*
* Class: OpenGL Sprite Sheet Loader
* Author: Robert Ciborowski
* Date: 01/04/2018
* Description: A class used to load an image into several smaller textures.
*              When accessing the textures, if the original image was named "char.png",
*              ask the resource manager for "char_X_Y.png", where X and Y are the coordinates.
*/

#pragma once
#include "GLTextureLoader.h"

namespace Aela {
	class GLSpriteSheetLoader : public GLTextureLoader {
		public:
			GLSpriteSheetLoader();
			~GLSpriteSheetLoader() override;

			bool load(ResourceMap& resources, std::string src) override;

			void setSpriteWidth(unsigned int spriteWidth);
			void setSpriteHeight(unsigned int spriteHeight);

		protected:
			bool loadPNGSpriteSheet(ResourceMap& resources, std::string src);

		private:
			unsigned int spriteWidth = 1;
			unsigned int spriteHeight = 1;
	};
}