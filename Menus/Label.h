/*
* Class: Label
* Author: Robert Ciborowski
* Date: 22/08/2017
* Description: A class which represents a line of text.
*/

#pragma once
#include "Component.h"

namespace Aela {
	class Label : public Component {
		public:
			Label(std::string text, Font* font, unsigned int size);
			Label(std::string text, Font* font, unsigned int size, ColourRGBA* colour);
			Label(std::string text, Font* font, unsigned int size, PositioningMode2D positioningMode);
			Label(std::string text, Font* font, unsigned int size, ColourRGBA* colour, PositioningMode2D positioningMode);

			virtual ~Label();

			// These are getters and setters.
			void setText(std::string text);
			std::string& getText();
			void setFont(Font* font);
			Font* getFont();
			void setSize(unsigned int size);
			unsigned int getSize();
			void setColour(ColourRGBA* colour);
			ColourRGBA* getColour();
			virtual void renderWithDifferentTint(GLRenderer& renderer, ColourRGBA* tint);
			void useFontSize();
			void setBuffer(std::vector<unsigned char>* buffer);
			std::vector<unsigned char>* getBuffer();
			void setBufferWidth(unsigned int bufferWidth);
			unsigned int getBufferWidth();
			void setBufferHeight(unsigned int bufferHeight);
			unsigned int getBufferHeight();
			void setNeedToReCreateBuffer(bool needToRecreateBuffer);
			bool getNeedToRecreateBuffer();
			void setCharacterPositioning(Rect<float>* characterPositioning);
			Rect<float>* getCharacterPositioning();

		protected:
			std::string text;
			Font* font;
			unsigned int size;
			ColourRGBA colour;

			void setupWidthAndHeight();
			virtual void updateComponent();
			virtual void renderComponent(GLRenderer& renderer);

		private:
			std::vector<unsigned char> buffer;
			unsigned int bufferWidth = 0, bufferHeight = 0;
			bool needToRecreateBuffer = true;
			Rect<float> characterPositioning;

			void setup();
	};
}
