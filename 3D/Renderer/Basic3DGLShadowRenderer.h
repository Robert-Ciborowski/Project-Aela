/*
* Name: Project Aela's Basic 3D Shadow GLRenderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D GLRenderer to render shadow maps for Transformables.
*/

#pragma once
#include "../Models/ModelEntity.h"
#include "../Light/LightEntity.h"
#include "../Maps/Map3D.h"

#include <map>

namespace Aela {
	class Basic3DGLShadowRenderer {
		public:
			Basic3DGLShadowRenderer();
			~Basic3DGLShadowRenderer();

			void setup();

			void startRenderingShadows(GLuint depthProgramID);
			void renderInstancedShadows(Map3D* map, std::vector<long long>* entities, size_t start, size_t end,
				GLuint depthProgramID, GLuint shadowModelMatrixID, GLuint shadowMatrixID,
				std::unordered_map<long long, LightEntity>* lights, GLuint lightPositionsID);
			void endRenderingShadows();

			// This adds a model's shadow to the shadow map. Don't use this function anymore, its deprecated!
			void renderUninstancedShadow(ModelEntity* entity, GLuint depthProgramID, GLuint shadowModelMatrixID,
				GLuint shadowMatrixID, std::unordered_map<long long, LightEntity>* lights, GLuint lightPositionsID);

			// This clears all shadow maps of lights.
			void clearShadowMaps(std::unordered_map<long long, LightEntity>* lights);

			void generateShadowMap(LightEntity* light);

			// These are getters and setters.
			unsigned int getDepthTextureWidth();
			unsigned int getDepthTextureHeight();

			// This is for debugging.
			void renderTestCube();

		private:
			const unsigned int DEPTH_TEXTURE_WIDTH = 2048, DEPTH_TEXTURE_HEIGHT = 2048;
			const unsigned int MAX_LIGHT_AMOUNT = 5;

			GLuint vertexBuffer = 0, normalBuffer = 0, elementBuffer = 0;
	};
}