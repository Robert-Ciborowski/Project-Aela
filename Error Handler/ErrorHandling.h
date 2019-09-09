/*
* Namespace: Project Aela's Easy-To-Use Error Handler
* Author: Robert Ciborowski
* Date: October 2016
* Description: A namespace with classes that handles error messages.
*/

#pragma once
#include <string>
#include <functional>
#include <ft2build.h>
#include <freetype/freetype.h>

// This enum is used for the types of errors:
//		Console Window Errors:
//			Errors that appear in a seperate window.
//			The window looks like the Operating System's terminal.
//			Example: A window that looks like cmd in Windows.
//		Console Internal Errors:
//			Errors that appear in the engine's own console.
//			This console is accessible in-the-window/in-game.
//			Example: Try pressing '~' in Valve Source games.
//		Error Window Errors:
//			This displays a seperate error message
//			Example: A Windows error message with errorButtons and icons.

// This is the Error Handling namespace, which contains easy to use functions that output errors.
// The namespace uses Aela's error-related classes, which would more annoying to use by a programmer
// than using one function call to create an error.
namespace AelaErrorHandling {
	typedef void(*SignalHandlerPointer)(int);

	// These are the namespace's simple behaviours.
	void consoleWindowError(std::string message);
	void consoleWindowError(std::string setTitle, std::string message);
	void consoleWindowWarning(std::string message);
	void consoleWindowWarning(std::string setTitle, std::string message);

	void consoleInternalError(std::string message);
	void consoleInternalError(std::string setTitle, std::string message);

	void windowError(std::string message);
	void windowError(std::string setTitle, std::string message);
	void windowWarning(std::string message);
	void windowWarning(std::string setTitle, std::string message);

	// These should be used along with signal.h in order to throw exceptions that aren't normally thrown
	// by low-level erros (such as segment violations and abnormal program termination). See the following
	// for more information: https://www.tutorialspoint.com/c_standard_library/signal_h.htm
	void basicHandleSignal(int signal);
	SignalHandlerPointer handleSignal(int signalCode, SignalHandlerPointer handlingFunction);
	SignalHandlerPointer handleSignal(int signalCode);

	// This is used to decipher FreeType errors.
	const char* getFreeTypeErrorMessage(FT_Error err);

	bool programCloseWasRequested();
}
