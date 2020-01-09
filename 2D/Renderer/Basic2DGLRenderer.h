/*
* Class: Project Aela's 2D GLRenderer
* Author: Robert Ciborowski
* Date: February 2017
* Description: A class used by Aela's GLRenderer to render 2D objects.
*/

#pragma once

// These are the includes.
#include <vector>
#include <string>
#include <glm/glm.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

// These are Project Aela headers.
#include "Basic2DRenderer.h"
#include "../../Utilities/Rect/Rect.h"
#include "../../2D/Texture/GLImage.h"
#include "../Simple 2D Framebuffer/Simple2DFramebuffer.h"
#include "../Fonts/Font.h"
#include "../../Utilities/Colour/ColourRGBA.h"
#include "../../Window/Window.h"
#include "../Positioning Mode/PositioningMode2D.h"

// This is the Basic2DGLRenderer class.
namespace Aela {
	class Label;

	class Basic2DGLRenderer : public Basic2DRenderer {
		public:
			Basic2DGLRenderer();

			// This is the start function.
			void setup();

			// These functions are accessible to Project Aela's main renderer in order to render 2D components.
			void renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, ColourRGBA* tint,
				PositioningMode2D positioningMode);
			void renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, ColourRGBA* tint,
				PositioningMode2D positioningMode, unsigned int customShader);
			void renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<int>* bufferDimensions, ColourRGBA* tint,
				PositioningMode2D positioningMode);
			void renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<int>* bufferDimensions, ColourRGBA* tint,
				PositioningMode2D positioningMode, unsigned int customShader);
			void renderTextToSimple2DFramebuffer(std::string text, Font* font, unsigned int size, Simple2DFramebuffer* framebuffer, Rect<int>* output,
				ColourRGBA* colour, PositioningMode2D positioningMode, unsigned int pointsPerPixel);
			void renderTextToSimple2DFramebuffer(std::wstring& text, Font* font, unsigned int size, Simple2DFramebuffer* framebuffer, Rect<int>* output,
				ColourRGBA* colour, PositioningMode2D positioningMode, unsigned int pointsPerPixel);
			void renderLabelToSimple2DFramebuffer(Label* label, Simple2DFramebuffer* framebuffer, PositioningMode2D positioningMode,
				unsigned int pointsPerPixel);
			void renderMultisampledBufferToBuffer(unsigned int multisampledBuffer, unsigned int secondaryBuffer, Rect<int>* bufferDimensions);
			void renderRectangle(Rect<int>* output, Simple2DFramebuffer* framebuffer, ColourRGBA* colour, PositioningMode2D positioningMode);
			void renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, Simple2DFramebuffer* framebuffer, ColourRGBA* colour,
				PositioningMode2D positioningMode);
			void renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, Simple2DFramebuffer* framebuffer, ColourRGBA* colour);
			void renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX, unsigned int pointCY,
				  Simple2DFramebuffer* framebuffer, ColourRGBA* colour);

			// These are some useful, self-explanatory functions.
			void drawTestQuad();
			bool checkFrameBuffer();

			// Although this function is called in start(), it should be called to change the MSAA amount.
			void setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, unsigned int multisampling, Rect<int>* dimensions, Rect<int>* output);

			void clearSimple2DFramebuffer(Simple2DFramebuffer* framebuffer);

			void setTextScaling(unsigned int textScaling);
			unsigned int getTextScaling();

		private:
			// These are handles to shaders.
			unsigned int bufferTextureToBufferProgramID, textToBufferProgramID, imageToBufferProgramID;
			// These are handles to variables inside of the image shader. Note: shaders that use the same uniforms and buffers as the
			// 2DTextureBufferToBuffer shader (bufferTextureToBufferProgramID) should set the same locations of uniforms as they are in
			// the 2DTextureBufferToBuffer shader.
			unsigned int imageTextureID, imageVertexBufferID, imageUVBufferID, imageTopLeftCoordID, imageWindowDimensionsID, imageTintID;
			// These are handles to variables inside of the text shader as well as the actual texture used for text.
			unsigned int characterTextureID, characterTexture, characterQuadVertexBufferID, characterColourID, characterAntialiasingID,
				characterUVBufferID;

			// This function is used to render a single character.
			void renderCharacterBuffer(Simple2DFramebuffer* framebuffer, Rect<float>* output, Rect<int>* outputBufferDimensions,
				std::vector<unsigned char>* buffer, unsigned int width, unsigned int rows, ColourRGBA* colour, bool antialiasing);

			// These functions are used in the start of the 2D renderer.
			void load2DShaders();
			void getGLSLVariableHandles();

			// This is used for text super sampling.
			unsigned int textScaling = 1;

			bool shadersAreSetup = false;

			struct RenderableGlyph {
				unsigned char* buffer;
				int width, rows, originY;
				wchar_t character;
			};
	};
}
