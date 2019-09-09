/*
* Class: Project Aela's Error Handler Class
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class that creates errors.
*/

#include "AelaErrorHandler.h"
#include <iostream>

void AelaErrorHandler::requestProgramClose() {
	programShouldBeClosed = true;
}

void AelaErrorHandler::throwError(AelaSimpleError error) {
	AelaErrorMessageType type = error.getType();
	if (type == AELA_ERROR_CONSOLE_WINDOW) {
		std::cout << "Aela Engine Error! " << error.getTitle() << ": " << error.getMessage() << "\n";
	} else if (type == AELA_WARNING_CONSOLE_WINDOW) {
		std::cout << "Aela Engine Message! " << error.getTitle() << ": " << error.getMessage() << "\n";
	} else if (type == AELA_ERROR_CONSOLE_INTERNAL) {
		// Implementation for an internal console within the engine/game must be added.
	} else if (type == AELA_ERROR_WINDOW) {
		std::cout << "Aela Engine Error! " << error.getTitle() << ": " << error.getMessage() << "\n";

		// This must save the error message text as a string and then converting to a c string.
		// Doing error.getTitle().c_str() returns nothing for some amazing reason.
		std::string titleAsString = error.getTitle();
		const char* titleAsChars = titleAsString.c_str();
		std::string messageAsString = error.getMessage();
		const char* messageAsChars = messageAsString.c_str();

		SDL_MessageBoxData messageboxdata = {
			SDL_MESSAGEBOX_INFORMATION, /* .flags */
			NULL, /* .window */
			titleAsChars, /* .title */
			messageAsChars, /* .message */
			numberOfErrorButtons, /* .numbuttons */
			errorButtons, /* .errorButtons */
			&colorScheme /* .colorScheme */
		};

		std::cout << error.getTitle().c_str() << "\n";
		int buttonid;
		if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
			SDL_Log("error displaying message box");
		}
		if (buttonid == -1) {
			SDL_Log("no selection");
		} else if (errorButtons[buttonid].text == "End Program") {
			requestProgramClose();
		}
	} else if (type == AELA_WARNING_WINDOW) {
		std::cout << "Aela Engine Warning! " << error.getTitle() << ": " << error.getMessage() << "\n";

		// This must save the error message text as a string and then converting to a c string.
		// Doing error.getTitle().c_str() returns nothing for some amazing reason.
		std::string titleAsString = error.getTitle();
		const char* titleAsChars = titleAsString.c_str();
		std::string messageAsString = error.getMessage();
		const char* messageAsChars = messageAsString.c_str();

		SDL_MessageBoxData messageboxdata = {
			SDL_MESSAGEBOX_INFORMATION, /* .flags */
			NULL, /* .window */
			titleAsChars, /* .title */
			messageAsChars, /* .message */
			numberOfWarningButtons, /* .numbuttons */
			warningButtons, /* .errorButtons */
			&colorScheme /* .colorScheme */
		};

		int buttonid;
		if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
			SDL_Log("error displaying message box");
		}
		if (buttonid == -1) {
			SDL_Log("no selection");
		}
	} else {
		// This should be unreachable code.
	}
}