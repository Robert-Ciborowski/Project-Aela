/*
* Class: Basic 3D Model GLRenderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D GLRenderer to specifically render 3D models.
*/

#include "../Models/ModelEntity.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../Window/Window.h"
#include "../Light/LightEntity.h"
#include "../Maps/Map3D.h"

#include <map>

namespace Aela {
	class Basic3DGLModelRenderer {
		public:
			Basic3DGLModelRenderer();
			~Basic3DGLModelRenderer();

			void setup();

			void startRendering(GLuint modelProgramID, GLuint frameBuffer, GLuint viewMatrixID, GLuint projectionMatrixID);
			void renderInstancedModelEntities(Map3D* map, std::vector<long long>* entities, size_t start, size_t end,
				GLuint modelProgramID, GLuint frameBuffer, GLuint modelMatrixID, GLuint rotationMatrixID, GLuint modelTextureID,
				GLuint ambientLightingID);
			void endRendering();

			// This function renders a model. It requires a lot of GLuints that are provided by
			// the Basic3DGLRenderer.
			void renderUninstancedModelEntity(ModelEntity* entity, GLuint frameBuffer, GLuint modelProgramID, GLuint modelMVPMatrixID, GLuint modelMatrixID,
				GLuint viewMatrixID, GLuint modelRotationMatrixID, GLuint modelTextureID, GLuint cameraPositionID, glm::vec3* cameraPosition);

			// These functions render a 2D texture in 3D space.
			void renderTextureIn3DSpace(bool cullFaces, GLuint texture, GLuint billboardTextureID,
				GLuint programID, GLuint frameBuffer, GLuint billboardMVPMatrixID, glm::vec3* position, glm::vec3* scale, glm::vec3* lookAt, bool inverseRotation);
			void renderTextureIn3DSpace(bool cullFaces, GLuint texture, GLuint billboardTextureID,
				GLuint programID, GLuint frameBuffer, GLuint billboardMVPMatrixID, glm::vec3* position, glm::vec3* rotation, glm::vec3* scale);

			// This is made for the Basic3DGLRenderer in order to set camera matrices.
			void setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix);

			// This sends light data to the model shader.
			void sendLightDataToShader(std::unordered_map<long long, LightEntity>* lights, GLuint modelProgramID, GLuint numberOfLightsID, GLuint lightPositionsID,
				GLuint lightDirectionsID, GLuint lightColoursID, GLuint lightPowersID, GLuint shadowMapID);

		private:
			// These are the only properties necessary for this class.
			glm::mat4 viewMatrix, projectionMatrix;
			const unsigned int MAX_LIGHT_AMOUNT = 5;

			GLuint vertexBuffer = 0, UVBuffer = 0, normalBuffer = 0, elementBuffer = 0, whichMatrixBuffer = 0;

			// These functions are here for debugging purposes.
			void drawTestQuad();
			void renderTestCube();
	};
}