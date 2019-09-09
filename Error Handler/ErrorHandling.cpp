/*
* Namespace: Project Aela's Error Handler
* Author: Robert Ciborowski
* Date: October 2016
* Description: A namespace with classes that handles error messages.
*/

#include "ErrorHandling.h"
#include "Error Handler/AelaErrorHandler.h"
#include "Simple Error/AelaSimpleError.h"
#include <iostream>
#include <signal.h>

namespace AelaErrorHandling {
	AelaErrorHandler errorHandler;
}

void AelaErrorHandling::consoleWindowError(std::string message) {
	// This outputs an error into the seperate window that acts as a console.
	AelaSimpleError simpleError(AELA_ERROR_CONSOLE_WINDOW, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::consoleWindowError(std::string title, std::string message) {
	// This outputs an error into the seperate window that acts as a console.
	AelaSimpleError simpleError(AELA_ERROR_CONSOLE_WINDOW, title, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::consoleWindowWarning(std::string message) {
	// This outputs an error into the seperate window that acts as a console.
	AelaSimpleError simpleError(AELA_WARNING_CONSOLE_WINDOW, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::consoleWindowWarning(std::string title, std::string message) {
	// This outputs an error into the seperate window that acts as a console.
	AelaSimpleError simpleError(AELA_WARNING_CONSOLE_WINDOW, title, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::consoleInternalError(std::string message) {
	// Implementation for an internal console within the engine/game must be added.
	AelaSimpleError simpleError(AELA_ERROR_CONSOLE_INTERNAL, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::consoleInternalError(std::string title, std::string message) {
	// Implementation for an internal console within the engine/game must be added.
	AelaSimpleError simpleError(AELA_ERROR_CONSOLE_INTERNAL, title, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::windowError(std::string message) {
	// This outputs an error using the operating system's error message window.
	AelaSimpleError simpleError(AELA_ERROR_WINDOW, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::windowError(std::string title, std::string message) {
	// This outputs an error using the operating system's error message window.
	AelaSimpleError simpleError(AELA_ERROR_WINDOW, title, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::windowWarning(std::string message) {
	// This outputs an error using the operating system's error message window.
	AelaSimpleError simpleError(AELA_WARNING_WINDOW, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::windowWarning(std::string title, std::string message) {
	// This outputs an error using the operating system's error message window.
	AelaSimpleError simpleError(AELA_WARNING_WINDOW, title, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::basicHandleSignal(int signal) {
	// std::cout << "Signal " << signal << " has been received!";
	throw "Signal";
}

AelaErrorHandling::SignalHandlerPointer AelaErrorHandling::handleSignal(int signalCode, SignalHandlerPointer handlingFunction) {
	return signal(signalCode, handlingFunction);
}

AelaErrorHandling::SignalHandlerPointer AelaErrorHandling::handleSignal(int signalCode) {
	return signal(signalCode, basicHandleSignal);
}

const char* AelaErrorHandling::getFreeTypeErrorMessage(FT_Error err) {
		#undef __FTERRORS_H__
		#define FT_ERRORDEF( e, v, s )  case e: return s;
		#define FT_ERROR_START_LIST     switch (err) {
		#define FT_ERROR_END_LIST       }
		#include FT_ERRORS_H
		return "(Unknown error)";
}

bool AelaErrorHandling::programCloseWasRequested() {
	return errorHandler.programShouldBeClosed;
}
