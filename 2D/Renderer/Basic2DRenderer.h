/*
* Class: Project Aela's 2D Renderer
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used by Aela's Renderer to render 2D objects.
*/

#pragma once

// These are the includes.
#include <vector>
#include <string>
// #include <glm/glm.hpp>

// These are Project Aela headers.
#include "../../Utilities/Rect/Rect.h"
#include "../../2D/Texture/GLImage.h"
#include "../Simple 2D Framebuffer/Simple2DFramebuffer.h"
#include "../Fonts/Font.h"
#include "../../Utilities/Colour/ColourRGBA.h"
#include "../../Window/Window.h"
#include "../Positioning Mode/PositioningMode2D.h"

// This is the Basic2DRenderer class.
namespace Aela {
	class Label;

	class Basic2DRenderer {
		public:
			Basic2DRenderer();

			~Basic2DRenderer();

			// This is the start function.
			virtual void setup() = 0;

			// These functions are accessible to Project Aela's main renderer in order to render 2D components.
			virtual void renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, ColourRGBA* tint,
				PositioningMode2D positioningMode) = 0;
			virtual void renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, ColourRGBA* tint,
				PositioningMode2D positioningMode, unsigned int customShader) = 0;
			virtual void renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<int>* bufferDimensions, ColourRGBA* tint,
				PositioningMode2D positioningMode) = 0;
			virtual void renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<int>* bufferDimensions, ColourRGBA* tint,
				PositioningMode2D positioningMode, unsigned int customShader) = 0;
			virtual void renderTextToSimple2DFramebuffer(std::string text, Font* Font, unsigned int size, Simple2DFramebuffer* framebuffer, Rect<int>* output, ColourRGBA* colour,
				PositioningMode2D positioningMode, unsigned int pointsPerPixel) = 0;
			virtual void renderLabelToSimple2DFramebuffer(Label* label, Simple2DFramebuffer* framebuffer, PositioningMode2D positioningMode,
				unsigned int pointsPerPixel) = 0;
			virtual void renderMultisampledBufferToBuffer(unsigned int multisampledBuffer, unsigned int secondaryBuffer, Rect<int>* bufferDimensions) = 0;
			virtual void renderRectangle(Rect<int>* output, Simple2DFramebuffer* framebuffer, ColourRGBA* colour,
				PositioningMode2D positioningMode) = 0;
			virtual void renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, Simple2DFramebuffer* framebuffer, ColourRGBA* colour,
				PositioningMode2D positioningMode) = 0;
			virtual void renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, Simple2DFramebuffer* framebuffer, ColourRGBA* colour) = 0;
			virtual void renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX, unsigned int pointCY,
				  Simple2DFramebuffer* framebuffer, ColourRGBA* colour) = 0;

			// These are some useful, self-explanatory functions.
			virtual void drawTestQuad() = 0;
			virtual bool checkFrameBuffer() = 0;

			// Although this function is called in start(), it should be called to change the MSAA amount.
			virtual void setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, unsigned int multisampling, Rect<int>* dimensions, Rect<int>* output) = 0;

			virtual void clearSimple2DFramebuffer(Simple2DFramebuffer* framebuffer) = 0;

			// These are some getters and setters.
			void setWindow(Window* setWindow);
			Window* getWindow();

		protected:
			// These are some Project Aela objects that the 3D renderer uses.
			Window* window = nullptr;

			// These are used by the renderer so that "window->getDimensions()->getWidth()"
			// (and "...getHeight()") does not have to called all the time.
			int windowWidth = 0;
			int windowHeight = 0;
	};
}