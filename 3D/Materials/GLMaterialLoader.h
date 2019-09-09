/*
* Class: OpenGL Material Loader
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for loading material files.
*              Warning: Because obj files only refer to materials by their name
*              (and not their full path), the OBJ Loader can only ask the Resource
*              Manager for these materials using their names alone. This means that
*              materials are added to the Resource Manager using their name, and not
*              their full path!
*/

#pragma once

#include "../../Resource Management/ResourceLoader.h"
#include "../../2D/Texture/GLTextureLoader.h"

namespace Aela {
	class GLMaterialLoader : public GLTextureLoader {
		public:
			GLMaterialLoader();
			virtual ~GLMaterialLoader();

			virtual bool load(ResourceMap& resources, std::string src);

		private:
			unsigned int MATERIAL_PATH_START_POSITION = 7;
			unsigned int MATERIAL_NAME_START_POSITION = 7;
			const std::string defaultPathForTextures = "res/materials";
	};
}