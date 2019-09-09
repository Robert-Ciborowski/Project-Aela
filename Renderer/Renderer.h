/*
* Class: Renderer
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used for representing a renderer, generically.
*/

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <atomic>

// This makes GLEW static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

// Note: The different types of ParticleEmitters already include "GLBillboardEntity.h".
//#include "../3D/Particles/PlanarParticleEmitter.h"
#include "../3D/Particles/PlanarParticleEmitter.h"
#include "../3D/Maps/Map3D.h"
#include "../2D/Simple 2D Framebuffer/Simple2DFramebuffer.h"
#include "../Utilities/Colour/ColourRGBA.h"
#include "../Window/Window.h"
#include "../3D/Camera/Camera3D.h"
#include "../Control Manager/ControlManager.h"
#include "../3D/Light/LightEntity.h"
#include "../3D/Models/ModelEntity.h"
#include "../3D/Skybox/SkyboxEntity.h"
#include "../Events/EventListener.h"
#include "../Events/EventHandler.h"
#include "../Events/KeyEvent.h"
#include "../2D/Positioning Mode/PositioningMode2D.h"
#include "../2D/Fonts/Font.h"

// These are some enums used by the Renderer.
namespace Aela {
	class Label;

	// This enum is used to toggle features of the renderer.
	enum class RendererFeature {
		SHADOWS, BILLBOARDS, SKYBOX, LIGHTS,
		MSAA_3D_X0, MSAA_3D_X2, MSAA_3D_X4, MSAA_3D_X8, MSAA_3D_X16,
		MSAA_2D_X0, MSAA_2D_X2, MSAA_2D_X4, MSAA_2D_X8, MSAA_2D_X16,
		SSAA_TEXT_X1, SSAA_TEXT_X2, SSAA_TEXT_X4, SSAA_TEXT_X6, SSAA_TEXT_X8, SSAA_TEXT_X16
	};

	enum class RenderingAPI {
		OPEN_GL, VULKAN
	};

	class Renderer {
		public:
			Renderer();
			Renderer(Window* window);
			Renderer(const Renderer&);

			~Renderer() {}

			// These functions initialize required elements for different types of rendering.
			// They MUST be called before performing their type of rendering.
			virtual void setup3D() = 0;
			virtual void setup2D() = 0;

			// This clears the window to black and makes it shown.
			virtual void setupWindow() = 0;

			virtual RenderingAPI getRenderingAPI() = 0;

			void onEvent(Event* event);

			// These functions bind the Renderer with other Aela classes. They must be called
			// before rendering.
			void setWindow(Window* window);
			void setTime(Clock* time);
			void setEventHandler(EventHandler* eventHandler);

			virtual void renderMap(Map3D* map, unsigned int skybox) = 0;

			virtual void renderBillboard(BillboardEntity* billboard) = 0;
			virtual void renderSkybox(SkyboxEntity* skybox) = 0;
			virtual void renderParticles(ParticleEmitter* particleEmitter) = 0;
			virtual void generateShadowMap(LightEntity* light) = 0;
			virtual void startRendering3D() = 0;

			// These functions are related specifically to the simple 2D frame buffer.
			virtual void setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, Rect<int>* dimensions, Rect<int>* output) = 0;
			virtual void clearSimple2DFramebuffer() = 0;

			// These functions are related to 2D rendering.
			virtual void render2DImage(Image* image, Rect<int>* output, Rect<int>* cropping, ColourRGBA* tint, PositioningMode2D positioningMode) = 0;
			virtual void renderText(std::string text, Font* font, unsigned int size, Rect<int>* output, ColourRGBA* colour, PositioningMode2D positioningMode) = 0;
			virtual void renderLabel(Label* label, PositioningMode2D positioningMode) = 0;
			virtual void renderRectangle(Rect<int>* output, ColourRGBA* colour, PositioningMode2D positioningMode) = 0;
			virtual void renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, ColourRGBA* colour, PositioningMode2D positioningMode) = 0;
			virtual void renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, ColourRGBA* colour) = 0;
			virtual void renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX,
				unsigned int pointCY, ColourRGBA* colour) = 0;
			virtual void renderSimple2DFramebuffer() = 0;

			// These functions are used to render a frame.
			virtual void startRenderingFrame() = 0;
			virtual void endRendering3D() = 0;
			virtual void endRenderingFrame() = 0;

			// These functions are for (de)activating renderer features. A library-specific renderer might want to
			// perform library-specific operations after a feature is toggled.
			virtual void activateFeature(RendererFeature feature) = 0;
			virtual void deactivateFeature(RendererFeature feature) = 0;
			virtual void toggleFeature(RendererFeature feature) = 0;
			virtual bool isUsingFeature(RendererFeature feature) = 0;

			unsigned int getMultisampling3D();
			unsigned int getMultisampling2D();

			// This sets the field of view of the bounded camera.
			void setFOV(float value);

			void set3DTint(ColourRGBA* tint3D);
			void clear3DTint();

			// This allows for only a certain portion of a framebuffer to be rendered to.
			virtual void scissor(int x, int y, size_t width, size_t height);
			virtual void resetScissor();

			virtual void resolutionWasChanged();

			virtual void setOutputToWindow(Rect<int>* outputToWindow);
			Rect<int>* getOutputToWindow();
			void outputToEntireWindow();

			// These are some getters.
			Window* getWindow();
			Clock* getClock();
			Camera3D* getCamera();

		protected:
			// These are a bunch of Project Aela objects that the renderer uses.
			Camera3D camera;
			Clock* time = nullptr;
			Window* window = nullptr;
			EventHandler* eventHandler;

			// This is a general tint.
			ColourRGBA tint3D;

			// If the output is meant to only be put into a certain portion of the window (such as during letterboxing),
			// this should represent that output. By default, this will equal the window dimensions (so that the full
			// window is rendered to).
			Rect<int> outputToWindow;

			// These specify the features that the renderer is allowed to use during rendering.
			std::unordered_map<RendererFeature, bool> features;
			std::atomic<bool> resolutionWasChangedFlag = { false };
			unsigned int multisampling3D = 0, multisampling2D = 0;

			virtual void resetResolution() = 0;
	};
};
