/*
* Class: Particle Loader
* Author: Robert Ciborowski
* Date: 02/08/2017
* Description: A class that is made to load particles.
*/

#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#include "../../2D/Texture/GLTextureLoader.h"

namespace Aela {
	class GLParticleLoader : public GLTextureLoader {
		public:
			GLParticleLoader();
			virtual ~GLParticleLoader();

			virtual bool load(std::unordered_map<std::string, Resource*>* resources, std::string src);
	};
}
