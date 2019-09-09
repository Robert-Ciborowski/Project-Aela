/*
* Name: Project Aela's Basic 3D Skybox GLRenderer
* Author: Robert Ciborowski
* Date: 21/06/2017
* Description: A class used by Project Aela's Basic 3D GLRenderer to render a skybox using a cubemap.
*/

#include "../Skybox/Skybox.h"
#include "../Camera/Camera3D.h"

namespace Aela {
	class Basic3DSkyboxRenderer {
		public:
			Basic3DSkyboxRenderer();
			~Basic3DSkyboxRenderer();

			void setup();

			// This adds a model's shadow to the shadow map.
			void renderSkybox(Aela::Skybox* skybox, GLuint skyboxProgramID, GLuint frameBuffer, GLuint skyboxID, GLuint viewMatrixID, GLuint projectionMatrixID);

			// This sets the matrices of the camera.
			void setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix);

		private:
			glm::mat4 viewMatrix, projectionMatrix;
			GLuint vertexBuffer = 0, vertexArray = 0;

			// These are the vertex values of the skybox. They never change.
			const float vertexValues[108] = {
				-1.0f, 1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,

				-1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,

				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,

				-1.0f, 1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, 1.0f
			};
	};
}