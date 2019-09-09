#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include "GLSkyboxLoader.h"
#include "Skybox.h"
#include "./../../Error Handler/ErrorHandling.h"

using namespace Aela;

const std::string GLSkyboxLoader::paths[] = {
	"/right.dds",
	"/left.dds",
	"/up.dds",
	"/down.dds",
	"/back.dds",
	"/front.dds"
};

GLSkyboxLoader::GLSkyboxLoader() {}

GLSkyboxLoader::~GLSkyboxLoader() {}

bool GLSkyboxLoader::load(ResourceMap& resources, std::string src) {
	Skybox* res = new Skybox(src);

	// I'm removing this feature for now, since it's rather important to be consistent throughout Aela in resource conventions.
	// In addition, this currently does not save any memory since an extra property was added to every resource.
	// Please talk to me about re-adding this feature if you need it.

	/*// This checks to see if the resource can use the "~" shortcut for its path to signify that the resource is using the
	// commonly excepted path for its resource type. Doing this saves memory.
	if (src.substr(0, defaultTexturePath.size()) == defaultTexturePath) {
		result->setSource("~" + src.substr(defaultTexturePath.size(), src.size()));
	} else {
		result->setSource(src);
	}*/

	GLuint* cubeMapTexture = res->getTexture();
	glGenTextures(1, cubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMapTexture);

	for (unsigned int i = 0; i < 6; i++) {
		/*std::ifstream in;
		if (!open(in, src + paths[i])) {*/
		std::ifstream in(src + paths[i], std::ios::binary);
		if (!isValid(in)) {
			AelaErrorHandling::consoleWindowError("Aela Skybox Loader", "The skybox " + src + paths[i] + " was not found.");
			return false;
		}

		/*if (!loadDDSUsingFILE(src + paths[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i)) {
			return false;
		}*/
		// loadTexture(in, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
		loadDDSToBoundId(in, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

		// in.close();
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	resources.put(src, res);
	return true;
}
