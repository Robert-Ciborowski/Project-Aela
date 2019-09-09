#pragma once

#include "../../2D/Texture/GLTextureLoader.h"

namespace Aela {
	class GLSkyboxLoader : public Aela::GLTextureLoader {
		public:
			static const std::string paths[];

			GLSkyboxLoader();
			virtual ~GLSkyboxLoader();

			GLSkyboxLoader(GLSkyboxLoader const&) = delete;
			void operator=(GLSkyboxLoader const&) = delete;

			virtual bool load(ResourceMap& resources, std::string src);
	};
}
