/*
* Class: Basic 3D GLRenderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Aela's GLRenderer that uses other, more basic objects
*              to render 3D items.
*/

#include "Basic3DGLRenderer.h"
#include "../../Utilities/glmut.h"
#include <glm/gtx/component_wise.hpp>

using namespace Aela;

// This sets up the class.
void Basic3DGLRenderer::setup(unsigned int multisampling) {
	if (!shadersAreSetup) {
		setupShaders();
		shadersAreSetup = true;
	}
	setupFrameBuffers(multisampling);
	getIDs();

	shadowRenderer.setup();
	modelRenderer.setup();
	skyboxRenderer.setup();
}

// This creates and compiles the shaders.
void Basic3DGLRenderer::setupShaders() {
	depthProgramID = loadShaders("../../../res/shaders/3D/Depth.vert", "../../../res/shaders/3D/Depth.geom", "../../../res/shaders/3D/Depth.frag");
	modelProgramID = loadShaders("../../../res/shaders/3D/Model.vert", "../../../res/shaders/3D/Model.frag");
	lightlessModelProgramID = loadShaders("../../../res/shaders/3D/Model.vert", "../../../res/shaders/3D/Model_Without_Lights.frag");
	billboardProgramID = loadShaders("../../../res/shaders/3D/Billboards.vert", "../../../res/shaders/3D/Billboards.frag");
	skyboxProgramID = loadShaders("../../../res/shaders/3D/Skybox.vert", "../../../res/shaders/3D/Skybox.frag");
}

// This gets handles to GLSL uniforms.
void Basic3DGLRenderer::getIDs() {
	modelTextureID = glGetUniformLocation(modelProgramID, "textureSampler");
	projectionMatrixID = glGetUniformLocation(modelProgramID, "projectionMatrix");
	viewMatrixID = glGetUniformLocation(modelProgramID, "viewMatrix");
	modelMatrixID = glGetUniformLocation(modelProgramID, "modelMatrices");
	rotationMatrixID = glGetUniformLocation(modelProgramID, "rotationMatrices");
	shadowMapID = glGetUniformLocation(modelProgramID, "shadowMaps");
	numberOfLightsID = glGetUniformLocation(modelProgramID, "numberOfLights");
	// ambientLightingID = glGetUniformLocation(modelProgramID, "ambientLighting");
	ambientLightingID = glGetUniformLocation(modelProgramID, "ambientLighting");

	lightlessModelTextureID = glGetUniformLocation(lightlessModelProgramID, "textureSampler");
	lightlessGlobalLightingID = glGetUniformLocation(lightlessModelProgramID, "globalLighting");
	lightlessProjectionMatrixID = glGetUniformLocation(lightlessModelProgramID, "projectionMatrix");
	lightlessViewMatrixID = glGetUniformLocation(lightlessModelProgramID, "viewMatrix");
	lightlessModelMatrixID = glGetUniformLocation(lightlessModelProgramID, "modelMatrices");
	lightlessRotationMatrixID = glGetUniformLocation(lightlessModelProgramID, "rotationMatrices");

	// I have no clue as to why, but openGL does not return the proper uniform address unless I name these
	// uniforms EXACTLY like this.
	lightPositionsID = glGetUniformLocation(modelProgramID, "openGLSucksAtPositions[0]");
	lightDirectionsID = glGetUniformLocation(modelProgramID, "lightDirections");
	lightColoursID = glGetUniformLocation(modelProgramID, "openGLSucksAtColours[0]");
	lightPowersID = glGetUniformLocation(modelProgramID, "openGLSucksAtLightPowers[0]");

	billboardTextureID = glGetUniformLocation(billboardProgramID, "textureSampler");
	billboardMVPMatrixID = glGetUniformLocation(billboardProgramID, "modelViewProjectionMatrix");

	shadowMatrixID = glGetUniformLocation(depthProgramID, "shadowMatrices");
	shadowModelMatrixID = glGetUniformLocation(depthProgramID, "modelMatrices");
	lightShadowPositionsID = glGetUniformLocation(depthProgramID, "lightPosition");

	skyboxTextureID = glGetUniformLocation(skyboxProgramID, "skyboxTexture");
	skyboxViewMatrixID = glGetUniformLocation(skyboxProgramID, "viewMatrix");
	skyboxProjectionMatrixID = glGetUniformLocation(skyboxProgramID, "projectionMatrix");
}

// This sets up all of the framebuffers, including the MSAA framebuffer.
// It is also called if the MSAA amount changes.
void Basic3DGLRenderer::setupFrameBuffers(unsigned int multisampling) {
	if (colourFrameBuffer != 0) {
		glDeleteFramebuffers(1, &colourFrameBuffer);
		glDeleteTextures(1, colourFrameBufferTexture.getTexture());
	}

	// This generates the colour framebuffer.
	glGenFramebuffers(1, &colourFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, colourFrameBuffer);

	glGenTextures(1, colourFrameBufferTexture.getTexture());
	glBindTexture(GL_TEXTURE_2D, *(colourFrameBufferTexture.getTexture()));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, outputWidth, outputHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Clamping to edges is important to prevent artifacts when scaling.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Linear filtering usually looks best for text.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	colourFrameBufferTexture.setDimensions(0, 0, outputWidth, outputHeight);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(colourFrameBufferTexture.getTexture()), 0);

	if (multisampling > 0) {
		if (multisampledColourFrameBuffer != 0) {
			glDeleteFramebuffers(1, &multisampledColourFrameBuffer);
			glDeleteTextures(1, multisampledColourFrameBufferTexture.getTexture());
		}

		// This generates the multisampled colour framebuffer, which later is blitted to the regular colour framebuffer.
		glGenFramebuffers(1, &multisampledColourFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, multisampledColourFrameBuffer);

		glGenTextures(1, multisampledColourFrameBufferTexture.getTexture());
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *(multisampledColourFrameBufferTexture.getTexture()));
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, outputWidth, outputHeight, GL_TRUE);

		// Clamping to edges is important to prevent artifacts when scaling.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Linear filtering usually looks best for text.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		multisampledColourFrameBufferTexture.setDimensions(0, 0, outputWidth, outputHeight);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, *(multisampledColourFrameBufferTexture.getTexture()), 0);
	} else {
		// If the MSAA amount is changed from a value greater than zero to zero, the buffer and texture should be deleted.
		glDeleteBuffers(1, &multisampledColourFrameBuffer);
		glDeleteTextures(1, multisampledColourFrameBufferTexture.getTexture());
	}

	if (depthRenderBuffer != 0) {
		glDeleteRenderbuffers(1, &depthRenderBuffer);
	}

	// This generates the depth renderbuffer, which is used alongside the multisampled colour framebuffer.
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);

	if (multisampling > 0) {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, outputWidth, outputHeight);
	} else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, outputWidth, outputHeight);
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Project Aela 3D Rendering",
			"There was a problem setting movingUp the colour framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
	}
}

void Aela::Basic3DGLRenderer::rebuildFrameBuffers(bool multisampling) {
	glDeleteFramebuffers(1, &colourFrameBuffer);
	glDeleteTextures(1, colourFrameBufferTexture.getTexture());
	glDeleteRenderbuffers(1, &depthRenderBuffer);

	if (multisampling) {
		glDeleteBuffers(1, &multisampledColourFrameBuffer);
		glDeleteTextures(1, multisampledColourFrameBufferTexture.getTexture());
	}

	setupFrameBuffers(multisampling);
}

void Aela::Basic3DGLRenderer::endRendering() {
	modelRenderer.endRendering();
}

// This generates a light's depth frame buffer.
void Basic3DGLRenderer::generateShadowMap(LightEntity* light) {
	shadowRenderer.generateShadowMap(light);
}

void Basic3DGLRenderer::setWindow(Window* setWindow) {
	window = setWindow;
}

void Basic3DGLRenderer::setCamera(Camera3D* setCamera) {
	camera = setCamera;
}

void Basic3DGLRenderer::bindLights(std::unordered_map<long long, LightEntity>* lights) {
	this->lights = lights;
}

void Aela::Basic3DGLRenderer::prepareLightShadows() {
	for (auto& pair : *lights) {
		if (!pair.second.isSetUpForShadows()) {
			shadowRenderer.generateShadowMap(&pair.second);
		}
	}
}

Window* Basic3DGLRenderer::getWindow() {
	return window;
}

GLuint* Basic3DGLRenderer::getColourFrameBuffer() {
	return &colourFrameBuffer;
}

Image* Basic3DGLRenderer::getColourFrameBufferTexture() {
	return &colourFrameBufferTexture;
}

GLuint * Aela::Basic3DGLRenderer::getMultisampledColourFrameBuffer() {
	return &multisampledColourFrameBuffer;
}

Image* Aela::Basic3DGLRenderer::getMultisampledColourFrameBufferTexture() {
	return &multisampledColourFrameBufferTexture;
}

void Basic3DGLRenderer::setOutputWidthAndHeight(int outputWidth, int outputHeight) {
	this->outputWidth = outputWidth;
	this->outputHeight = outputHeight;
}

Aela::Basic3DGLRenderer::Basic3DGLRenderer() {}

Aela::Basic3DGLRenderer::~Basic3DGLRenderer() {
	// This cleans all VBOs and shaders.
	glDeleteProgram(modelProgramID);
	glDeleteProgram(depthProgramID);
	glDeleteProgram(lightlessModelProgramID);
	glDeleteProgram(billboardProgramID);
	glDeleteProgram(skyboxProgramID);
}

void Aela::Basic3DGLRenderer::renderShadows(Map3D* map) {
	glViewport(0, 0, shadowRenderer.getDepthTextureWidth(), shadowRenderer.getDepthTextureHeight());

	shadowRenderer.startRenderingShadows(depthProgramID);
	for (auto& pair : *map->getResourceGroupedModelsWithoutTransparency()) {
		for (size_t start = 0; start < pair.second.size(); start += SHADOW_INSTANCE_LIMIT) {
			size_t end = start + SHADOW_INSTANCE_LIMIT;
			if (end > pair.second.size()) {
				end = pair.second.size();
			}
			shadowRenderer.renderInstancedShadows(map, &pair.second, start, end, depthProgramID, shadowModelMatrixID, shadowMatrixID,
				lights, lightShadowPositionsID);
		}
	}
	shadowRenderer.endRenderingShadows();
}

void Aela::Basic3DGLRenderer::renderModelEntitiesWithLights(Map3D* map, bool multisampling) {
	glViewport(0, 0, outputWidth, outputHeight);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());

	if (multisampling) {
		modelRenderer.startRendering(modelProgramID, multisampledColourFrameBuffer, viewMatrixID, projectionMatrixID);
	} else {
		modelRenderer.startRendering(modelProgramID, colourFrameBuffer, viewMatrixID, projectionMatrixID);
	}

	for (auto& pair : *map->getResourceGroupedModelsWithoutTransparency()) {
		for (size_t start = 0; start < pair.second.size(); start += MODEL_INSTANCE_LIMIT) {
			size_t end = start + MODEL_INSTANCE_LIMIT;
			if (end > pair.second.size()) {
				end = pair.second.size();
			}
			if (multisampling) {
				modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, modelProgramID, multisampledColourFrameBuffer,
					modelMatrixID, rotationMatrixID, modelTextureID, ambientLightingID);
			} else {
				modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, modelProgramID, colourFrameBuffer,
					modelMatrixID, rotationMatrixID, modelTextureID, ambientLightingID);
			}
		}
	}

	for (auto& layer : *map->getResourceGroupedModelsWithTransparency()) {
		for (auto& pair : *layer.second) {
			for (size_t start = 0; start < pair.second.size(); start += MODEL_INSTANCE_LIMIT) {
				size_t end = start + MODEL_INSTANCE_LIMIT;
				if (end > pair.second.size()) {
					end = pair.second.size();
				}
				if (multisampling) {
					modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, modelProgramID, multisampledColourFrameBuffer,
						modelMatrixID, rotationMatrixID, modelTextureID, ambientLightingID);
				} else {
					modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, modelProgramID, colourFrameBuffer,
						modelMatrixID, rotationMatrixID, modelTextureID, ambientLightingID);
				}
			}
		}
	}
}

void Aela::Basic3DGLRenderer::renderModelEntitiesWithoutLights(Map3D* map, bool multisampling) {
	glViewport(0, 0, outputWidth, outputHeight);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());

	if (multisampling) {
		modelRenderer.startRendering(lightlessModelProgramID, multisampledColourFrameBuffer, lightlessViewMatrixID, lightlessProjectionMatrixID);
	} else {
		modelRenderer.startRendering(lightlessModelProgramID, colourFrameBuffer, lightlessViewMatrixID, lightlessProjectionMatrixID);
	}

	for (auto& pair : *map->getResourceGroupedModelsWithoutTransparency()) {
		for (size_t start = 0; start < pair.second.size(); start += MODEL_INSTANCE_LIMIT) {
			size_t end = start + MODEL_INSTANCE_LIMIT;
			if (end > pair.second.size()) {
				end = pair.second.size();
			}
			if (multisampling) {
				modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, lightlessModelProgramID, multisampledColourFrameBuffer,
					lightlessModelMatrixID, lightlessRotationMatrixID, lightlessModelTextureID, lightlessGlobalLightingID);
			} else {
				modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, lightlessModelProgramID, colourFrameBuffer,
					lightlessModelMatrixID, lightlessRotationMatrixID, lightlessModelTextureID, lightlessGlobalLightingID);
			}
		}
	}

	for (auto& layer : *map->getResourceGroupedModelsWithTransparency()) {
		for (auto& pair : *layer.second) {
			for (size_t start = 0; start < pair.second.size(); start += MODEL_INSTANCE_LIMIT) {
				size_t end = start + MODEL_INSTANCE_LIMIT;
				if (end > pair.second.size()) {
					end = pair.second.size();
				}
				if (multisampling) {
					modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, lightlessModelProgramID, multisampledColourFrameBuffer,
						lightlessModelMatrixID, lightlessRotationMatrixID, lightlessModelTextureID, lightlessGlobalLightingID);
				} else {
					modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, lightlessModelProgramID, colourFrameBuffer,
						lightlessModelMatrixID, lightlessRotationMatrixID, lightlessModelTextureID, lightlessGlobalLightingID);
				}
			}
		}
	}
}

//void Aela::Basic3DGLRenderer::renderSingleModelEntityShadow(ModelEntity* entity, Map3D* map) {
//	shadowRenderer.renderUninstancedShadow(entity, depthProgramID, shadowModelMatrixID,
//		shadowMatrixID, map->getLights(), lightShadowPositionsID);
//}

// This clears the colour frame buffer.
void Basic3DGLRenderer::clearColourFrameBuffer(bool multisampling) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if (multisampling) {
		glBindFramebuffer(GL_FRAMEBUFFER, multisampledColourFrameBuffer);
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, colourFrameBuffer);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// This renders a texture in 3D space.
void Basic3DGLRenderer::renderTextureIn3DSpace(GLuint* texture, bool cullFaces, glm::vec3* position, glm::vec3* scaling, glm::vec3* lookAt, bool inverseRotation, bool multisampling) {
	// Note: for regular texture rendering, use:
	// renderTextureIn3DSpace((texture, false, position, position + glm::vec3(0.0, 0.0, 1.0), false);
	glViewport(0, 0, outputWidth, outputHeight);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	if (multisampling) {
		modelRenderer.renderTextureIn3DSpace(cullFaces, *texture, billboardTextureID, billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, position, scaling, lookAt, inverseRotation);
	} else {
		modelRenderer.renderTextureIn3DSpace(cullFaces, *texture, billboardTextureID, billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, position, scaling, lookAt, inverseRotation);
	}
}

// This renders a billboard, accounting for multisampling.
void Basic3DGLRenderer::renderBillboard(BillboardEntity* billboard, bool multisampling) {
	if (billboard->getTexture() != nullptr) {
		glViewport(0, 0, outputWidth, outputHeight);
		modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
		if (billboard->usingSpecifiedRotation()) {
			if (multisampling) {
				modelRenderer.renderTextureIn3DSpace(true, *billboard->getTexture()->getTexture(), billboardTextureID,
					billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, billboard->getPosition(),
					billboard->getRotation(), billboard->getScaling());
			} else {
				modelRenderer.renderTextureIn3DSpace(true, *billboard->getTexture()->getTexture(), billboardTextureID,
					billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, billboard->getPosition(),
					billboard->getRotation(), billboard->getScaling());
			}
		} else {
			if (multisampling) {
				modelRenderer.renderTextureIn3DSpace(true, *billboard->getTexture()->getTexture(), billboardTextureID,
					billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, billboard->getPosition(),
					billboard->getScaling(), camera->getPosition(), true);
			} else {
				modelRenderer.renderTextureIn3DSpace(true, *billboard->getTexture()->getTexture(), billboardTextureID,
					billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, billboard->getPosition(),
					billboard->getScaling(), camera->getPosition(), true);
			}
		}
	}
}

void Basic3DGLRenderer::clearShadowMaps() {
	shadowRenderer.clearShadowMaps(lights);
}

// This function tells the renderer to send the lights added through bindLights() to the shaders.
void Basic3DGLRenderer::sendLightDataToShader() {
	modelRenderer.sendLightDataToShader(lights, modelProgramID, numberOfLightsID, lightPositionsID, lightDirectionsID, lightColoursID, lightPowersID, shadowMapID);
}

// This renders a skybox, accounting for multisampling.
void Basic3DGLRenderer::renderSkybox(Skybox* skybox, bool multisampling) {
	glViewport(0, 0, outputWidth, outputHeight);
	skyboxRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	if (multisampling) {
		skyboxRenderer.renderSkybox(skybox, skyboxProgramID, multisampledColourFrameBuffer, skyboxTextureID, skyboxViewMatrixID, skyboxProjectionMatrixID);
	} else {
		skyboxRenderer.renderSkybox(skybox, skyboxProgramID, colourFrameBuffer, skyboxTextureID, skyboxViewMatrixID, skyboxProjectionMatrixID);
	}
}

// This renders particles. Keep in mind that particles are sorted in the ParticleEmitter's list (based on their z position, relative to the 
// emitter). This function uses a (probably slow) technique to figure out whether the particles should be rendered according to their order in the
// list or according the backwards version of this order. This is necessary since if billboards (which is what a particle is) are not rendered
// from the greatest z-distance to the smallest z distance relative to position 0, 0, 0 in the camera space, their alpha blending will screw up and
// look bad.
void Aela::Basic3DGLRenderer::renderParticles(ParticleEmitter* particleEmitter, Camera3D* camera, bool multisampling) {
	glViewport(0, 0, outputWidth, outputHeight);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	glm::vec3 actualCameraRotation = glm::vec3(camera->getRotation()->y, camera->getRotation()->x, camera->getRotation()->z);
	glm::vec3 differenceA = actualCameraRotation - *particleEmitter->getRotation();
	glm::vec3 differenceB = *particleEmitter->getRotation() - actualCameraRotation;
	forceWithinRange(&differenceA, 0, glm::pi<float>() * 2);
	forceWithinRange(&differenceB, 0, glm::pi<float>() * 2);
	glm::vec3 difference = glm::vec3(glm::min(differenceA.x, differenceB.x), glm::min(differenceA.y, differenceB.y),
		glm::min(differenceA.z, differenceB.z));
	float angle = glm::compMax(difference);

	glm::vec3 fakeOffset;

	if (angle < glm::pi<float>() / 2) {
		for (Particle particle : *particleEmitter->getParticles()) {
			renderParticle(particle, particleEmitter->getPosition(), particleEmitter->getRotation(), particleEmitter->getScaling(),
				multisampling);
		}
	} else {
		for (size_t i = particleEmitter->getParticles()->size() - 1; i > 0; i--) {
			renderParticle(particleEmitter->getParticles()->at(i), particleEmitter->getPosition(), particleEmitter->getRotation(),
				particleEmitter->getScaling(), multisampling);
		}
	}
}

void Aela::Basic3DGLRenderer::renderParticle(Particle particle, glm::vec3* positionOffset, glm::vec3* rotationOffset, glm::vec3* scalingOffset,
	bool multisampling) {
	glm::vec3 position = *particle.getPosition() + *positionOffset;
	glm::vec3 rotation = *particle.getRotation() + *rotationOffset;
	glm::vec3 scaling = *particle.getScaling() * *scalingOffset;

	if (particle.usingSpecifiedRotation()) {
		if (multisampling) {
			modelRenderer.renderTextureIn3DSpace(false, *particle.getTexture()->getTexture(), billboardTextureID, billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, &position, &rotation, &scaling);
		} else {
			modelRenderer.renderTextureIn3DSpace(false, *particle.getTexture()->getTexture(), billboardTextureID, billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, &position, &rotation, &scaling);
		}
	} else {
		if (multisampling) {
			modelRenderer.renderTextureIn3DSpace(false, *particle.getTexture()->getTexture(), billboardTextureID, billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, &position, &scaling, camera->getPosition(), true);
		} else {
			modelRenderer.renderTextureIn3DSpace(false, *particle.getTexture()->getTexture(), billboardTextureID, billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, &position, &scaling, camera->getPosition(), true);
		}
	}
}
