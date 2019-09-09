/*
* Name: Project Aela's Shadow GLRenderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D GLRenderer to render shadows of 3D models.
*/

#include "Basic3DGLShadowRenderer.h"
#include "../../Error Handler/ErrorHandling.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>

using namespace Aela;

// This clears all shadow maps that would be rendered.
void Basic3DGLShadowRenderer::clearShadowMaps(std::unordered_map<long long, LightEntity>* lights) {
	if (lights != nullptr) {
		unsigned int i = 0;
		for (auto& light : *lights) {
			if (light.second.isSetUpForShadows()) {
				if (i + 1 > MAX_LIGHT_AMOUNT) {
					break;
				}
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glBindFramebuffer(GL_FRAMEBUFFER, *light.second.getShadowMapBuffer());
				glClear(GL_DEPTH_BUFFER_BIT);
				i++;
			}
		}
	}
}

void Aela::Basic3DGLShadowRenderer::generateShadowMap(LightEntity* light) {
	GLuint* buffer = light->getShadowMapBuffer();
	glGenFramebuffers(1, buffer);

	// This generates the depth texture, which is sampled from later when rendering models.
	GLuint* texture = light->getShadowMapTexture();
	glGenTextures(1, texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, DEPTH_TEXTURE_WIDTH,
			DEPTH_TEXTURE_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glBindFramebuffer(GL_FRAMEBUFFER, *buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *texture, 0);

	// There is no colour output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela 3D",
			(std::string) "There was a problem setting movingUp the depth framebuffer.\nIt's probably OpenGL's fault.\n"
			+ "It's possible that you ran out of VRAM.\nMaybe your graphics processor is a potato.");
		light->setShadowMapBuffer(NULL);
		light->setShadowMapTexture(NULL);
	} else {
		light->setShadowMapBuffer(buffer);
		light->setShadowMapTexture(texture);
	}

	light->hasBeenSetUpForShadows();
}

Aela::Basic3DGLShadowRenderer::Basic3DGLShadowRenderer() {}

Aela::Basic3DGLShadowRenderer::~Basic3DGLShadowRenderer() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &elementBuffer);
}

void Aela::Basic3DGLShadowRenderer::setup() {
	if (vertexBuffer != 0) {
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &elementBuffer);
	}
	// This sets up necessary buffers.
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &elementBuffer);
}

void Basic3DGLShadowRenderer::startRenderingShadows(GLuint depthProgramID) {
	// This enables face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	// glDisable(GL_CULL_FACE);

	glUseProgram(depthProgramID);

	// This sets up necessary buffers.
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &elementBuffer);

	// These are the vertex attributes.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// This is the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
}

void Basic3DGLShadowRenderer::renderInstancedShadows(Map3D* map, std::vector<long long>* entities, size_t start,
	size_t end, GLuint depthProgramID, GLuint shadowModelMatrixID, GLuint shadowMatrixID,
	std::unordered_map<long long, LightEntity>* lights, GLuint lightPositionsID) {
	if (entities != nullptr && entities->size() > 0) {
		std::vector<glm::mat4> modelMatrices;

		size_t primitiveCount = 0;
		for (size_t i = start; i < end; i++) {
			auto entity = map->getModel(entities->at(i));

			if (!entity->isVisible()) {
				continue;
			}

			primitiveCount++;

			// This is positioning/rotation of the subModel.
			glm::vec3* position = entity->getPosition();
			glm::vec3* rotation = entity->getRotation();
			glm::vec3* scaling = entity->getScaling();

			// This computes more matrices.
			modelMatrices.push_back(glm::scale(glm::translate(glm::mat4(1.0), *position), *scaling)
				* glm::eulerAngleYXZ(rotation->y, rotation->x, rotation->z));
		}

		// This sends all transformations to the shader.
		glUniformMatrix4fv(shadowModelMatrixID, (GLsizei) modelMatrices.size(), GL_FALSE, &modelMatrices[0][0][0]);

		unsigned int i = 0;
		for (auto& light : *lights) {
			if (!light.second.isSetUpForShadows()) {
				continue;
			}

			if (i + 1 > MAX_LIGHT_AMOUNT) {
				break;
			}

			// This calculates more matrices.
			float near = 1, far = 100;
			glm::vec3* lightPosition = light.second.getPosition();
			glm::mat4 depthProjectionMatrix = glm::perspective(glm::radians(90.0f), (float) DEPTH_TEXTURE_WIDTH / (float) DEPTH_TEXTURE_HEIGHT,
				near, far);
			glm::mat4 shadowTransformations[6];
			shadowTransformations[0] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			shadowTransformations[1] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			shadowTransformations[2] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			shadowTransformations[3] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			shadowTransformations[4] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			shadowTransformations[5] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

			glBindFramebuffer(GL_FRAMEBUFFER, *light.second.getShadowMapBuffer());
			glUniformMatrix4fv(shadowMatrixID, 6, GL_FALSE, &shadowTransformations[0][0][0]);
			glUniform3fv(lightPositionsID, 1, &(light.second.getPosition()->x));

			Model& model = *map->getModel(entities->at(start))->getModel();
			for (unsigned int whichSubModel = 0; whichSubModel < model.getSubModels()->size(); whichSubModel++) {
				SubModel& subModel = model.getSubModels()->at(whichSubModel);
				// This sends data to the buffers.
				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, (GLsizei) (subModel.getVertexSize() * sizeof(glm::vec3)), &subModel.getVertices()->at(0), GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei) (subModel.getIndexSize() * sizeof(unsigned short)), &subModel.getIndices()->at(0), GL_STATIC_DRAW);

				// This draws the elements.
				glDrawElementsInstanced(GL_TRIANGLES, (GLsizei) subModel.getIndexSize(), GL_UNSIGNED_SHORT, (void*) 0, (GLsizei) (primitiveCount));
			}
			i++;
		}
	}
}

void Basic3DGLShadowRenderer::endRenderingShadows() {
	glDisableVertexAttribArray(0);
}

// This renders a shadow of a entity to each light's depth buffer.
void Basic3DGLShadowRenderer::renderUninstancedShadow(ModelEntity* entity, GLuint depthProgramID,
	GLuint shadowModelMatrixID, GLuint shadowMatrixID, std::unordered_map<long long, LightEntity>* lights, GLuint lightPositionsID) {
	// This is old code that is only here for reference.
	//if (entity != nullptr && entity->getModel() != nullptr && entity->getModel()->getSubModels() != nullptr) {
	//	// This enables face culling.
	//	glEnable(GL_CULL_FACE);
	//	glCullFace(GL_FRONT);
	//	// glDisable(GL_CULL_FACE);

	//	glUseProgram(depthProgramID);

	//	// This sets up necessary buffers.
	//	GLuint vertexbuffer;
	//	glGenBuffers(1, &vertexbuffer);

	//	GLuint elementbuffer;
	//	glGenBuffers(1, &elementbuffer);

	//	/*std::cout << entity << " " << depthProgramID << " " << shadowModelMatrixID << " " << shadowMatrixID << " "
	//		<< lights << " " << lightPositionsID << "\n";*/

	//	// For some reason, range-based for loops break here so I had to use a regular for loop.
	//	for (unsigned int whichSubModel = 0; whichSubModel < entity->getModel()->getSubModels()->size(); whichSubModel++) {
	//		SubModel& subModel = entity->getModel()->getSubModels()->at(whichSubModel);
	//		unsigned int i = 0;
	//		for (auto& light : *lights) {
	//			if (!light.second.isSetUpForShadows()) {
	//				if (i + 1 > MAX_LIGHT_AMOUNT) {
	//					break;
	//				}

	//				// This sends data to the buffers.
	//				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//				glBufferData(GL_ARRAY_BUFFER, subModel.getVertexSize() * sizeof(glm::vec3), &subModel.getVertices()->at(0), GL_STATIC_DRAW);

	//				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//				glBufferData(GL_ELEMENT_ARRAY_BUFFER, subModel.getIndexSize() * sizeof(unsigned short), &subModel.getIndices()->at(0), GL_STATIC_DRAW);

	//				// These are positions and rotations for light and the subModel.
	//				glm::vec3* lightPosition = light.second.getPosition();
	//				glm::vec3* rotation = entity->getRotation();
	//				glm::vec3* position = entity->getPosition();
	//				glm::vec3* scaling = entity->getScaling();

	//				// This calculates more matrices.
	//				float near = 1, far = 100;
	//				glm::mat4 depthProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)DEPTH_TEXTURE_WIDTH / (float)DEPTH_TEXTURE_HEIGHT, near, far);
	//				glm::mat4 shadowTransformations[6];
	//				shadowTransformations[0] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//				shadowTransformations[1] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//				shadowTransformations[2] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//				shadowTransformations[3] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	//				shadowTransformations[4] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//				shadowTransformations[5] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	//				glBindFramebuffer(GL_FRAMEBUFFER, *light.second.getShadowMapBuffer());

	//				glUniformMatrix4fv(shadowMatrixID, 6, GL_FALSE, &shadowTransformations[0][0][0]);
	//				glUniform3fv(lightPositionsID, 1, &(light.second.getPosition()->x));

	//				// This computes more matrices.
	//				glm::mat4 modelMatrix = glm::scale(glm::translate(glm::mat4(1.0), *position), *scaling) * glm::eulerAngleYXZ(rotation->y, rotation->x, rotation->z);

	//				// This sends all transformations to the shader.
	//				glUniformMatrix4fv(shadowModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

	//				// These are the vertex attributes.
	//				glEnableVertexAttribArray(0);
	//				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//				// This is the index buffer.
	//				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	//				// This draws the elements.
	//				glDrawElements(GL_TRIANGLES, (GLsizei) subModel.getIndexSize(), GL_UNSIGNED_SHORT, (void*)0);

	//				// This frees the buffers from the memory. Delete this if you want to screw over your users.
	//				glDisableVertexAttribArray(0);
	//				i++;
	//			}
	//		}
	//	}

	//	glDeleteBuffers(1, &vertexbuffer);
	//	glDeleteBuffers(1, &elementbuffer);
	//}
}

unsigned int Basic3DGLShadowRenderer::getDepthTextureWidth() {
	return DEPTH_TEXTURE_WIDTH;
}

unsigned int Basic3DGLShadowRenderer::getDepthTextureHeight() {
	return DEPTH_TEXTURE_HEIGHT;
}

// This is for debugging.
void Basic3DGLShadowRenderer::renderTestCube() {
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	if (cubeVAO == 0) {
		// Order: back face, front face, movingLeft face, right face, bottom face, top face
		float vertices[] = {
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

			1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}