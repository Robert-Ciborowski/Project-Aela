/*
* Class: Project Aela's Error Class
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class that can represent various types of errors.
*/

#pragma once
#include <string>

// This enum is used internally by the AelaErrorHandling namespace. It defines
// the types of errors.
enum AelaErrorMessageType {
	AELA_ERROR_CONSOLE_WINDOW, AELA_WARNING_CONSOLE_WINDOW, AELA_ERROR_CONSOLE_INTERNAL, AELA_ERROR_WINDOW, AELA_WARNING_WINDOW
};

class AelaSimpleError {
	private:
		AelaErrorMessageType errorMessageType;
		std::string message;
		std::string title;

	public:
		AelaSimpleError() {

		}

		AelaSimpleError(AelaErrorMessageType setType, std::string setMessage) {
			setProperties(setType, setMessage);
		}

		AelaSimpleError(AelaErrorMessageType setType, std::string setTitle, std::string setMessage) {
			setProperties(setType, setTitle, setMessage);
		}

		void setProperties(AelaErrorMessageType setType, std::string setMessage);
		void setProperties(AelaErrorMessageType setType, std::string setTitle, std::string setMessage);
		AelaErrorMessageType getType();
		std::string getMessage();
		std::string getTitle();
};