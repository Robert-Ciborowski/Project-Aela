/*
* Class: Project Aela's Error Handler Class
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class that creates errors.
*/

#pragma once
#include "../Simple Error/AelaSimpleError.h"
#include "SDL.h"

class AelaErrorHandler {
	private:
		const SDL_MessageBoxColorScheme colorScheme = {
			{
			    // SDL's Message Box Background Colour
				{255, 0, 0},
				// SDL's Message Box Text Colour
				{0, 255, 0},
				// SDL's Message Box Button Border Colour
				{255, 255, 0},
				// SDL's Message Box Button Background Colour
				{0, 0, 255},
				// SDL's Message Box Button Selected Colour
				{255, 0, 255}
			}
		};

		const int numberOfErrorButtons = 2;

		const SDL_MessageBoxButtonData errorButtons[2] = {
			{ 0, 0, "End Program" },
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Okay" }
		};

		const int numberOfWarningButtons = 1;

		const SDL_MessageBoxButtonData warningButtons[2] = {
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Okay" }
		};


	public:
		bool programShouldBeClosed = false;

		AelaErrorHandler() {

		}

		void throwError(AelaSimpleError error);
		void requestProgramClose();
};
