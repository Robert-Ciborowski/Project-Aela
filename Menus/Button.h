/*
* Class: Button
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents a button. The onclick action may either be a function or an AelaEngineFunctor, but only
               one should be used.
*/

#pragma once

#include "ImageComponent.h"
#include "Label.h"

#include "../Events/EventHandler.h"
#include "../Events/MouseEvent.h"

namespace Aela {
	class Button : public ImageComponent {
		public:
			Button();
			Button(GLTexture* texture);
			Button(GLTexture* texture, Rect<int>* dimensions);
			Button(ColourRGBA* hoverTint, ColourRGBA* clickTint);
			virtual ~Button();

			// These are getters and setters.
			void setupOnClick(std::function<void()> function);
			void setText(std::wstring text);
			void setText(std::string text);
			std::wstring getText();
			std::string getTextAsString();
			void setHoverTint(ColourRGBA* hoverTint);
			void setClickTint(ColourRGBA* clickTint);
			void setFont(Font* font);
			void setFontSize(unsigned int size);
			void setTextColour(ColourRGBA* colour);
			void setDimensions(Rect<int>* dimensions);
			void setPosition(int x, int y);
			void setWidthAndHeight(int width, int height);

			virtual void updateComponent();
			virtual void renderComponent(GLRenderer& renderer);

			virtual void onMousePressed(MouseEvent* event);
			virtual void onMouseReleased(MouseEvent* event);
			virtual void onMouseEntered(MouseEvent* event);
			virtual void onMouseExited(MouseEvent* event);

			// This changes the button's width and height to match that of the text.
			void wrapAroundText();

		private:
			bool clickStarted = false, active = false;

			std::function<void()> onClick;

			// These are the tints used by the button. Keep in mind that a tint is a multiplier.
			ColourRGBA hoverTint, clickTint;

			Label textLabel;

			void setupLabelDimensions();
	};
}