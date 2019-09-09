#pragma once
#include <string>
#include <al.h>
#include "../Error Handler/ErrorHandling.h"

#define alDoWithErrorCheck_void(expr) {								\
	expr;															\
	std::string msg;												\
	if (checkALError(&msg)) {											\
		AelaErrorHandling::consoleWindowError("AudioPlayer", msg);	\
		return;														\
	}																\
}

#define alDoWithErrorCheck_noret(expr) {							\
	expr;															\
	std::string msg;												\
	if (checkALError(&msg)) {											\
		AelaErrorHandling::consoleWindowError("AudioPlayer", msg);	\
	}																\
}

static void parseErrorMessage(ALuint error, std::string* msg) {
	switch (error) {
	case AL_NO_ERROR:
		*msg = "No error occurred.";
		break;
	case AL_INVALID_NAME:
		*msg = "openAL received an invalid name as a parameter.";
		break;
	case AL_INVALID_ENUM:
		*msg = "openAL received an invalid enum as a parameter.";
		break;
	case AL_INVALID_VALUE:
		*msg = "openAL received an invalid value as a parameter.";
		break;
	case AL_INVALID_OPERATION:
		*msg = "The program attempted an invalid operation.";
		break;
	case AL_OUT_OF_MEMORY:
		*msg = "The program ran out of memory.";
		break;
	default:
		*msg = "An unknown error occurred.";
		break;
	}
}

static bool checkALError(std::string* msg) {
	ALenum error = alGetError();
	bool cond = error != AL_NO_ERROR;
	if (cond) {
		parseErrorMessage(error, msg);
	}
	return cond;
}