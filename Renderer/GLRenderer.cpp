/*
* Class: GLRenderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used for interacting between Project Aela's renderers.
*/

#include "GLRenderer.h"
#include "../Error Handler/ErrorHandling.h"
#include "../Old Garbage/shader.hpp"
#include "../Events/EventConstants.h"
#include <signal.h>

#if defined (_MSC_VER) || defined __MINGW32__
// Need this for DWORD and NVIDIA Optimus. Idk how to enable Optimus in order to use the dedicated GPU in Linux/Mac.
#include <windows.h>

// Force to use dedicated graphics. Maybe find a way to make this optional?
extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

#endif

using namespace Aela;

Aela::GLRenderer::GLRenderer() {
	windowFocus = false;
}

Aela::GLRenderer::GLRenderer(Window* windowToSet) {
	setWindow(windowToSet);
	basic3DRenderer.setCamera(&camera);
	setup3DRendering();
}

Aela::GLRenderer::~GLRenderer() {}

// This sets up 3D rendering, accounting for multisampling.
void GLRenderer::setup3DRendering() {
	if (mainFramebuffer == 0) {
		setupMainFrameBuffer();
	}
	basic3DRenderer.setup(multisampling3D);
}

// This sets up 2D rendering, accounting for multisampling.
void GLRenderer::setup2DRendering() {
	std::cout << mainFramebuffer << " is mainframebuffer\n";
	if (mainFramebuffer == 0) {
		setupMainFrameBuffer();
	}
	basic2DRenderer.setup();
}

// This sets up the main frame buffer.
void GLRenderer::setupMainFrameBuffer() {
	// This enables depth comparisons/depth testing.
	glEnable(GL_DEPTH_TEST);

	// This will accept a fragment if it closer to the camera than the previous one.
	glDepthFunc(GL_LESS);

	glGenFramebuffers(1, &mainFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);

	glGenTextures(1, mainFramebufferImage.getTexture());

	glBindTexture(GL_TEXTURE_2D, *(mainFramebufferImage.getTexture()));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window->getDimensions()->getWidth(), window->getDimensions()->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	mainFramebufferImage.setDimensions(0, 0, window->getDimensions()->getWidth(), window->getDimensions()->getHeight());
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(mainFramebufferImage.getTexture()), 0);
	checkFrameBuffer();

	// MSAA'd framebuffers will be rendered onto the main one, so we need to enable MSAA.
	glEnable(GL_MULTISAMPLE);
}

void Aela::GLRenderer::setupWindow() {
	startRenderingFrame();
	endRenderingFrame();
	window->show();
}

// This initializes GLEW.
bool GLRenderer::setupGLEW() {
	glewExperimental = static_cast<GLboolean>(true);
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		return false;
	}
	return true;
}

// This sets up the 3D elements.
void GLRenderer::setup3D() {
	if (window != nullptr) {
		setup3DRendering();
		if (effects3DShader == 0) {
			effects3DShader = loadShaders("../../../res/shaders/3D/3DPostProcessing.vert", "../../../res/shaders/3D/3DPostProcessing.frag");
		}
		basic3DRenderer.setCamera(&camera);
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The window must be set before setting movingUp 3D.");
	}
}

// This sets up the 2D elements.
void GLRenderer::setup2D() {
	if (window != nullptr) {
		setup2DRendering();
		if (effects2DShader == 0) {
			effects2DShader = loadShaders("../../../res/shaders/2D/2DPostProcessing.vert", "../../../res/shaders/2D/2DPostProcessing.frag");
		}
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The winodw must be set before setting movingUp 3D.");
	}
}

void GLRenderer::setWindow(Window* window) {
	Renderer::setWindow(window);
	basic3DRenderer.setWindow(window);
	basic2DRenderer.setWindow(window);
}

void GLRenderer::setTime(Clock* time) {
	this->time = time;
	camera.setTime(time);
}

RenderingAPI Aela::GLRenderer::getRenderingAPI() {
	return RenderingAPI::OPEN_GL;
}

void Aela::GLRenderer::bindLights(std::unordered_map<long long, LightEntity>* lights) {
	basic3DRenderer.bindLights(lights);
}

void Aela::GLRenderer::bindSimple2DFramebuffer(Simple2DFramebuffer* framebuffer) {
	bound2DFramebuffer = framebuffer;

	// This makes sure to sync the frambuffer's multisampling to the multisampling of the renderer.
	if (framebuffer->isUsingRendererMultsamplingLevel()) {
		framebuffer->setMultisampling(multisampling2D);
	}
}

// This starts the rendering of a frame.
void GLRenderer::startRenderingFrame() {
	if (resolutionWasChangedFlag) {
		resetResolution();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// This would clear the main frame buffer, but it doesn't need to be cleared since everything would get overwritten when writing the 3D and 2D
	// frame buffer textures at the end of the frame.
	// glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
	// glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	basic3DRenderer.clearColourFrameBuffer(multisampling3D > 0);
}

void Aela::GLRenderer::renderMap(Map3D* map, unsigned int skybox) {
	if (features[RendererFeature::SKYBOX]) {
		renderSkybox(map->getSkybox(skybox));
	}

	if (features[RendererFeature::LIGHTS]) {
		// Don't put this in the if statement below. Just trust me.
		basic3DRenderer.prepareLightShadows();

		if (features[RendererFeature::SHADOWS]) {
			basic3DRenderer.renderShadows(map);
		}

		sendBoundLightDataToShader();

		basic3DRenderer.renderModelEntitiesWithLights(map, multisampling3D != 0);
	} else {
		basic3DRenderer.renderModelEntitiesWithoutLights(map, multisampling3D != 0);
	}

	if (features[RendererFeature::BILLBOARDS]) {
		for (auto& entity : *map->getBillboards()) {
			renderBillboard(&entity.second);
		}
	}
}

// This tells the renderer to send the light data to the shaders that need the data.
void GLRenderer::sendBoundLightDataToShader() {
	basic3DRenderer.sendLightDataToShader();
}

// This renders a billboard, if billboards are enabled.
void GLRenderer::renderBillboard(BillboardEntity* billboard) {
	if (features[RendererFeature::BILLBOARDS]) {
		basic3DRenderer.renderBillboard(billboard, multisampling3D > 0);
	}
}

// This renders a skybox, if skyboxes are enabled.
void GLRenderer::renderSkybox(SkyboxEntity* skybox) {
	if (features[RendererFeature::SKYBOX]) {
		basic3DRenderer.renderSkybox(skybox->getSkybox(), multisampling3D > 0);
	}
}

void Aela::GLRenderer::renderParticles(ParticleEmitter* particleEmitter) {
	basic3DRenderer.renderParticles(particleEmitter, &camera, multisampling3D > 0);
}

// This renders a 2D texture using the 2D renderer.
void GLRenderer::render2DImage(Image* image, Rect<int>* output, Rect<int>* cropping, ColourRGBA* tint, PositioningMode2D positioningMode) {
	basic2DRenderer.renderImageToSimple2DFramebuffer(image, bound2DFramebuffer, output, cropping, tint, positioningMode);
}

// This renders text using the 2D renderer.
void GLRenderer::renderText(std::string text, Font* font, unsigned int size, Rect<int>* output, ColourRGBA* colour, PositioningMode2D positioningMode) {
    basic2DRenderer.renderTextToSimple2DFramebuffer(text, font, size, bound2DFramebuffer, output, colour, positioningMode, POINTS_PER_PIXEL);
}

void Aela::GLRenderer::renderLabel(Label* label, PositioningMode2D positioningMode) {
	basic2DRenderer.renderLabelToSimple2DFramebuffer(label, bound2DFramebuffer, positioningMode, POINTS_PER_PIXEL);
}

// This renders a rectangle using the 2D renderer.
void GLRenderer::renderRectangle(Rect<int>* output, ColourRGBA* colour, PositioningMode2D positioningMode) {
     basic2DRenderer.renderRectangle(output, bound2DFramebuffer, colour, positioningMode);
}

void GLRenderer::renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, ColourRGBA* colour, PositioningMode2D positioningMode) {
     basic2DRenderer.renderRectangle(xPosition, yPosition, width, height, bound2DFramebuffer, colour, positioningMode);
}

// This renders a triangle using the 2D renderer.
void GLRenderer::renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, ColourRGBA* colour) {
     basic2DRenderer.renderTriangle(pointA, pointB, pointC, bound2DFramebuffer, colour);
}

void GLRenderer::renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX,
	unsigned int pointCY, ColourRGBA * colour) {
     basic2DRenderer.renderTriangle(pointAX, pointAY, pointBX, pointBY, pointCX, pointCY, bound2DFramebuffer, colour);
}

void GLRenderer::renderSimple2DFramebuffer() {
	// basic2DRenderer.renderMultisampledBufferToBuffer(*framebuffer->getFramebuffer(), mainFramebuffer, window->getDimensions());
    if (bound2DFramebuffer->getMultisampling() > 0) {
        basic2DRenderer.renderMultisampledBufferToBuffer(*bound2DFramebuffer->getMultisampledFramebuffer(), *bound2DFramebuffer->getFramebuffer(), &outputToWindow);
    }

    // basic2DRenderer.renderImageToFramebuffer(bound2DFramebuffer->getFramebufferImage(), mainFramebuffer, &output2, &output, (Rect<unsigned int>*) &outputToWindow, nullptr, PositioningMode2D::TOP_LEFT);
    basic2DRenderer.renderImageToFramebuffer(bound2DFramebuffer->getFramebufferImage(), mainFramebuffer, (Rect<int>*) window->getDimensions(), (Rect<int>*) window->getDimensions(), (Rect<int>*) window->getDimensions(), nullptr, PositioningMode2D::TOP_LEFT, effects2DShader);
}

void GLRenderer::endRendering3D() {
	basic3DRenderer.endRendering();

    if (multisampling3D > 0) {
        basic2DRenderer.renderMultisampledBufferToBuffer(*basic3DRenderer.getMultisampledColourFrameBuffer(), *basic3DRenderer.getColourFrameBuffer(), (Rect<int>*) window->getDimensions());
    }


    basic2DRenderer.renderImageToFramebuffer(basic3DRenderer.getColourFrameBufferTexture(), mainFramebuffer, (Rect<int>*) window->getDimensions(), (Rect<int>*) window->getDimensions(), (Rect<int>*) window->getDimensions(), &tint3D, PositioningMode2D::TOP_LEFT, effects3DShader);
}

void GLRenderer::endRenderingFrame() {
    // glBindFramebuffer(GL_FRAMEBUFFER, 1);
    // glClearColor(0, 0.5, 0, 1);
    // glClear(GL_COLOR_BUFFER_BIT);

    Rect<int>* dimensions = (Rect<int>*) window->getDimensions();
    basic2DRenderer.renderImageToFramebuffer(&mainFramebufferImage, 0, &outputToWindow, dimensions, dimensions, nullptr, PositioningMode2D::TOP_LEFT);
    
	// basic2DRenderer.renderImageToFramebuffer(&mainFramebufferImage, 0, (Rect<int>*) window->getDimensions(), (Rect<int>*) window->getDimensions(), window->getDimensions(), nullptr, PositioningMode2D::TOP_LEFT);
    
	window->updateBuffer();
}

void GLRenderer::generateShadowMap(LightEntity* light) {
	// if (useShadows) {
		basic3DRenderer.generateShadowMap(light);
	// }
}

void GLRenderer::startRendering3D() {
	// if (useShadows) {
		basic3DRenderer.clearShadowMaps();
	// }
	camera.update();
	updateCameraMatrices();
}

void GLRenderer::setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, Rect<int>* dimensions, Rect<int>* output) {
	basic2DRenderer.setupSimple2DFramebuffer(framebuffer, multisampling2D, dimensions, output);
}

void GLRenderer::clearSimple2DFramebuffer() {
	basic2DRenderer.clearSimple2DFramebuffer(bound2DFramebuffer);
}

// This activates a rendering feature.
void GLRenderer::activateFeature(RendererFeature feature) {
	switch (feature) {
		case RendererFeature::SHADOWS:
			features[RendererFeature::SHADOWS] = true;
			break;
		case RendererFeature::BILLBOARDS:
			features[RendererFeature::BILLBOARDS] = true;
			break;
		case RendererFeature::SKYBOX:
			features[RendererFeature::SKYBOX] = true;
			break;
		case RendererFeature::LIGHTS:
			features[RendererFeature::LIGHTS] = true;
			break;
		case RendererFeature::MSAA_3D_X0:
			multisampling3D = 0;
			basic3DRenderer.rebuildFrameBuffers(false);
			break;
		case RendererFeature::MSAA_3D_X2:
			multisampling3D = 2;
			basic3DRenderer.rebuildFrameBuffers(true);
			break;
		case RendererFeature::MSAA_3D_X4:
			multisampling3D = 4;
			basic3DRenderer.rebuildFrameBuffers(true);
			break;
		case RendererFeature::MSAA_3D_X8:
			multisampling3D = 8;
			basic3DRenderer.rebuildFrameBuffers(true);
			break;
		case RendererFeature::MSAA_3D_X16:
			multisampling3D = 16;
			basic3DRenderer.rebuildFrameBuffers(true);
			break;
		case RendererFeature::MSAA_2D_X0:
			multisampling2D = 0;
			break;
		case RendererFeature::MSAA_2D_X2:
			multisampling2D = 2;
			break;
		case RendererFeature::MSAA_2D_X4:
			multisampling2D = 4;
			break;
		case RendererFeature::MSAA_2D_X8:
			multisampling2D = 8;
			break;
		case RendererFeature::MSAA_2D_X16:
			multisampling2D = 16;
			break;
		case RendererFeature::SSAA_TEXT_X1:
			basic2DRenderer.setTextScaling(1);
			break;
		case RendererFeature::SSAA_TEXT_X2:
			basic2DRenderer.setTextScaling(2);
			break;
		case RendererFeature::SSAA_TEXT_X4:
			basic2DRenderer.setTextScaling(4);
			break;
		case RendererFeature::SSAA_TEXT_X6:
			basic2DRenderer.setTextScaling(6);
			break;
		case RendererFeature::SSAA_TEXT_X8:
			basic2DRenderer.setTextScaling(8);
			break;
		case RendererFeature::SSAA_TEXT_X16:
			basic2DRenderer.setTextScaling(16);
			break;
	}
}

// This deactivates a rendering feature.
void GLRenderer::deactivateFeature(RendererFeature feature) {
	switch (feature) {
		case RendererFeature::SHADOWS:
			features[RendererFeature::SHADOWS] = false;
			break;
		case RendererFeature::BILLBOARDS:
			features[RendererFeature::BILLBOARDS] = false;
			break;
		case RendererFeature::SKYBOX:
			features[RendererFeature::SKYBOX] = false;
			break;
		case RendererFeature::LIGHTS:
			features[RendererFeature::LIGHTS] = false;
			break;
		case RendererFeature::MSAA_3D_X0:
			// You want to deactivate having no MSAA? What does that even mean?
			break;
		case RendererFeature::MSAA_3D_X2:
			if (multisampling3D == 2) {
				multisampling3D = 0;
				basic3DRenderer.rebuildFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_3D_X4:
			if (multisampling3D == 4) {
				multisampling3D = 0;
				basic3DRenderer.rebuildFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_3D_X8:
			if (multisampling3D == 8) {
				multisampling3D = 0;
				basic3DRenderer.rebuildFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_3D_X16:
			if (multisampling3D == 16) {
				multisampling3D = 0;
				basic3DRenderer.rebuildFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_2D_X0:
			// You want to deactivate having no MSAA? What does that even mean?
			// Enabling MSAA? But to what scaling factor?
			break;
		case RendererFeature::MSAA_2D_X2:
			if (multisampling2D == 2) {
				multisampling2D = 0;
			}
			break;
		case RendererFeature::MSAA_2D_X4:
			if (multisampling2D == 4) {
				multisampling2D = 0;
			}
			break;
		case RendererFeature::MSAA_2D_X8:
			if (multisampling2D == 8) {
				multisampling2D = 0;
			}
			break;
		case RendererFeature::MSAA_2D_X16:
			if (multisampling2D == 16) {
				multisampling2D = 0;
			}
			break;
		case RendererFeature::SSAA_TEXT_X1:
			// What would disabling no text supersampling mean?
			// Enabling supersampling? But to what scaling factor?
			break;
		case RendererFeature::SSAA_TEXT_X2:
			basic2DRenderer.setTextScaling(1);
			break;
		case RendererFeature::SSAA_TEXT_X4:
			basic2DRenderer.setTextScaling(1);
			break;
		case RendererFeature::SSAA_TEXT_X6:
			basic2DRenderer.setTextScaling(1);
			break;
		case RendererFeature::SSAA_TEXT_X8:
			basic2DRenderer.setTextScaling(1);
			break;
		case RendererFeature::SSAA_TEXT_X16:
			basic2DRenderer.setTextScaling(1);
			break;
	}
}

// This toggles a feature.
void Aela::GLRenderer::toggleFeature(RendererFeature feature) {
	switch (feature) {
		case RendererFeature::SHADOWS:
			features[RendererFeature::SHADOWS] = !features[RendererFeature::SHADOWS];
			break;
		case RendererFeature::BILLBOARDS:
			features[RendererFeature::BILLBOARDS] = !features[RendererFeature::BILLBOARDS];
			break;
		case RendererFeature::SKYBOX:
			features[RendererFeature::SKYBOX] = !features[RendererFeature::SKYBOX];
			break;
		case RendererFeature::LIGHTS:
			features[RendererFeature::LIGHTS] = !features[RendererFeature::LIGHTS];
			break;
	}
}

bool GLRenderer::isUsingFeature(RendererFeature feature) {
	switch (feature) {
		case RendererFeature::BILLBOARDS:
			return features[RendererFeature::BILLBOARDS];
		case RendererFeature::LIGHTS:
			return features[RendererFeature::LIGHTS];
		case RendererFeature::MSAA_2D_X0:
			return multisampling2D == 0;
		case RendererFeature::MSAA_2D_X2:
			return multisampling2D == 2;
		case RendererFeature::MSAA_2D_X4:
			return multisampling2D == 4;
		case RendererFeature::MSAA_2D_X8:
			return multisampling2D == 8;
		case RendererFeature::MSAA_2D_X16:
			return multisampling2D == 16;
		case RendererFeature::MSAA_3D_X0:
			return multisampling3D == 0;
		case RendererFeature::MSAA_3D_X2:
			return multisampling3D == 2;
		case RendererFeature::MSAA_3D_X4:
			return multisampling3D == 4;
		case RendererFeature::MSAA_3D_X8:
			return multisampling3D == 8;
		case RendererFeature::MSAA_3D_X16:
			return multisampling3D == 16;
		case RendererFeature::SHADOWS:
			return features[RendererFeature::SHADOWS];
		case RendererFeature::SKYBOX:
			return features[RendererFeature::SKYBOX];
		case RendererFeature::SSAA_TEXT_X1:
			return basic2DRenderer.getTextScaling() == 1;
		case RendererFeature::SSAA_TEXT_X2:
			return basic2DRenderer.getTextScaling() == 2;
		case RendererFeature::SSAA_TEXT_X4:
			return basic2DRenderer.getTextScaling() == 4;
		case RendererFeature::SSAA_TEXT_X6:
			return basic2DRenderer.getTextScaling() == 6;
		case RendererFeature::SSAA_TEXT_X8:
			return basic2DRenderer.getTextScaling() == 8;
		case RendererFeature::SSAA_TEXT_X16:
			return basic2DRenderer.getTextScaling() == 16;
		default:
			return false;
	}
}

void Aela::GLRenderer::scissor(int x, int y, size_t width, size_t height) {
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, (GLsizei) width, (GLsizei) height);
}

void Aela::GLRenderer::resetScissor() {
	glDisable(GL_SCISSOR_TEST);
}

void GLRenderer::resolutionWasChanged() {
	Renderer::resolutionWasChanged();
	setWindow(window);
	setTime(time);
	// setup3D();
	// setup2D();
	// setupWindow();
}

void GLRenderer::setOutputToWindow(Rect<int>* outputToWindow) {
	Renderer::setOutputToWindow(outputToWindow);
	int width = outputToWindow->getWidth(), height = outputToWindow->getHeight();
	basic3DRenderer.setOutputWidthAndHeight(width, height);
	basic3DRenderer.setup(multisampling3D);
}

void Aela::GLRenderer::updateCameraMatrices() {
	if (/*window->isFocused() && */camera.isInUse()) {
		// This calculates vectors for the cartesian-plane system. Note: It is important to calculate the right vector before the up vector as the up
		// vector is calculated using the right vector.
		camera.calculateCartesionalDirection();
		camera.calculateRightVector();
		camera.calculateUpVector();
		camera.calculateMatrices(outputToWindow.getWidth(), outputToWindow.getHeight());
	}
}

void Aela::GLRenderer::resetResolution() {
	resolutionWasChangedFlag = false;

	glDeleteFramebuffers(1, &mainFramebuffer);
	glDeleteTextures(1, mainFramebufferImage.getTexture());
	setupMainFrameBuffer();

	basic3DRenderer.setWindow(window);
	// basic3DRenderer.rebuildFrameBuffers(multisampling3D != 0);
}

// This is a useful function that checks the currently bound framebuffer to see if it was set up properly.
bool GLRenderer::checkFrameBuffer() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela Rendering",
			"There was a problem setting movingUp the framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
		return false;
	} else {
		return true;
	}
}

// This gets information from OpenGL, including information about the PC's graphics.
std::string GLRenderer::getInformation(GLRendererInformation infoToGet) {
	switch (infoToGet) {
		case GLRendererInformation::VENDOR:
			return std::string((char*) glGetString(GL_VENDOR));
		case GLRendererInformation::RENDERER:
			return std::string((char*) glGetString(GL_RENDERER));
		case GLRendererInformation::OPENGL_VERSION:
			return std::string((char*) glGetString(GL_VERSION));
		case GLRendererInformation::GLSL_VERSION:
			return std::string((char*) glGetString(GL_SHADING_LANGUAGE_VERSION));
//        case GLRendererInformation::OPENGL_EXTENSIONS:
//            return std::string((char*) glGetString(GL_EXTENSIONS));
		default:
			return "This information is not currently available.";
	}
}

Window* GLRenderer::getWindow() {
	return window;
}

Clock* GLRenderer::getClock() {
	return time;
}

Camera3D* GLRenderer::getCamera() {
	return &camera;
}
