/*
* Class: Project Aela's Error Class
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class that can represent various types of errors.
*/

#include "AelaSimpleError.h"

void AelaSimpleError::setProperties(AelaErrorMessageType setType, std::string setMessage) {
	errorMessageType = setType;
	message = setMessage;

	if (setType == AELA_WARNING_WINDOW) {
		title = "Aela Warning";
	} else {
		title = "Aela Error";
	}
}

void AelaSimpleError::setProperties(AelaErrorMessageType setType, std::string setTitle, std::string setMessage) {
	errorMessageType = setType;
	message = setMessage;
	title = setTitle;
}

AelaErrorMessageType AelaSimpleError::getType() {
	return errorMessageType;
}

std::string AelaSimpleError::getMessage() {
	return message;
}

std::string AelaSimpleError::getTitle() {
	return title;
}