/*
* Class: TextInput
* Author: Robert Ciborowski
* Date: 20/03/2017
* Description: A class which represents a text component. It allows the user to enter text.
*/

#pragma once
#include "Component.h"

namespace Aela {
	class TextInput : public Component {
		public:
			TextInput(std::string text, Font* font, unsigned int size);
			TextInput(std::string text, Font* font, unsigned int size, ColourRGBA* colour);

			virtual ~TextInput();

			void setup();

			// These are getters and setters.
			std::string getText();
			Font* getFont();
			ColourRGBA* getColour();

		protected:
			std::string text;
			int cursorPosition = 0;
			int selectionPosition = 0;

			Font* font;
			unsigned int size = 0;
			ColourRGBA colour;
			virtual void updateComponent();
			virtual void renderComponent(GLRenderer& renderer);
	};
}
