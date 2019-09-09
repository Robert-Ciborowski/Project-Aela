#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#include "../Old Garbage/shader.hpp"
#include "../Error Handler/ErrorHandling.h"

GLuint loadShaders(std::string vertexShaderPath, std::string fragmentShaderPath) {
	return loadShaders(vertexShaderPath, "", fragmentShaderPath);
}

GLuint loadShaders(std::string vertexShaderPath, std::string geometryShaderPath, std::string fragmentShaderPath) {
	printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

	// This creates the shaders.
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER), geometryShaderID,
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	if (geometryShaderPath != "") {
		geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	}

	// This reads the vertex shader code.
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexShaderPath);

	if (vertexShaderStream.is_open()) {
		std::string line;
		while (getline(vertexShaderStream, line)) {
			vertexShaderCode += "\n" + line;
		}
		vertexShaderStream.close();
	} else {
		AelaErrorHandling::windowError("Aela Shader Reader", "There was a problem finding the requested vertex shader:\n" + vertexShaderPath);
		return 0;
	}

	std::string geometryShaderCode;
	if (geometryShaderPath != "") {
		// This reads the geometry shader code.
		std::ifstream geometryShaderStream(geometryShaderPath);
		if (geometryShaderStream.is_open()) {
			std::string line = "";
			while (getline(geometryShaderStream, line)) {
				geometryShaderCode += "\n" + line;
			}
			geometryShaderStream.close();
		} else {
			AelaErrorHandling::windowError("Aela Shader Reader", "There was a problem finding the requested geometry shader:\n" + geometryShaderPath);
			return 0;
		}
	}

	// This reads the fragment shader code.
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentShaderPath);
	if (fragmentShaderStream.is_open()) {
		std::string line = "";
		while (getline(fragmentShaderStream, line)) {
			fragmentShaderCode += "\n" + line;
		}
		fragmentShaderStream.close();
	} else {
		AelaErrorHandling::windowError("Aela Shader Reader", "There was a problem finding the requested fragment shader:\n" + fragmentShaderPath);
		return 0;
	}

	GLint result = GL_FALSE;
	int logLength;

	// This compiles the vertex shader.
	AelaErrorHandling::consoleWindowWarning("Aela Shader Reader", "The vertex shader " + vertexShaderPath + " is compiling...");
	char const* vertexCodeAsChar = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexCodeAsChar, NULL);
	glCompileShader(vertexShaderID);

	// This retrieves any errors related to the vertex shader compilation.
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		std::vector<char> vertexShaderErrorMessage(logLength + 1);
		glGetShaderInfoLog(vertexShaderID, logLength, NULL, &vertexShaderErrorMessage[0]);
		std::string messageAsString(vertexShaderErrorMessage.begin(), vertexShaderErrorMessage.end());
		AelaErrorHandling::consoleWindowError("Aela Shader Reader", vertexShaderPath + " had an error: " + messageAsString);
	}

	if (geometryShaderPath != "") {
		// This compiles the geometry shader shader.
		AelaErrorHandling::consoleWindowWarning("Aela Shader Reader", "The geometry shader " + geometryShaderPath + " is compiling...");
		char const* geometryCodeAsChar = geometryShaderCode.c_str();
		glShaderSource(geometryShaderID, 1, &geometryCodeAsChar, NULL);
		glCompileShader(geometryShaderID);

		// This retrieves any errors related to the vertex shader compilation.
		glGetShaderiv(geometryShaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(geometryShaderID, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			std::vector<char> geometryShaderErrorMessage(logLength + 1);
			glGetShaderInfoLog(geometryShaderID, logLength, NULL, &geometryShaderErrorMessage[0]);
			std::string messageAsString(geometryShaderErrorMessage.begin(), geometryShaderErrorMessage.end());
			AelaErrorHandling::consoleWindowError("Aela Shader Reader", geometryShaderPath + " had an error: " + messageAsString);
		}
	}

	// This compiles the fragment shader.
	AelaErrorHandling::consoleWindowWarning("Aela Shader Reader", "The fragment shader " + fragmentShaderPath + " is compiling...");
	char const* fragmentCodeAsChar = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentCodeAsChar, NULL);
	glCompileShader(fragmentShaderID);

	// This retrieves any errors related to the fragment shader compilation.
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		std::vector<char> fragmentShaderErrorMessage(logLength + 1);
		glGetShaderInfoLog(fragmentShaderID, logLength, NULL, &fragmentShaderErrorMessage[0]);
		std::string messageAsString(fragmentShaderErrorMessage.begin(), fragmentShaderErrorMessage.end());
		AelaErrorHandling::consoleWindowError("Aela Shader Reader", fragmentShaderPath + " had an error: " + messageAsString);
	}

	// This links the shaders with OpenGL.
	AelaErrorHandling::consoleWindowWarning("Aela Shader Reader", "Aela Shader Reader is linking shaders with OpenGL.");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	if (geometryShaderPath != "") {
		glAttachShader(programID, geometryShaderID);
	}
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// This checks for any linking-related errors.
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		std::vector<char> finalProgramErrorMessage(logLength + 1);
		glGetProgramInfoLog(programID, logLength, NULL, &finalProgramErrorMessage[0]);
		std::string messageAsString(finalProgramErrorMessage.begin(), finalProgramErrorMessage.end());
		AelaErrorHandling::consoleWindowError("Aela Shader Reader", "Shader program linking had an error: " + messageAsString);
	}

	glDetachShader(programID, vertexShaderID);
	glDeleteShader(vertexShaderID);
	if (geometryShaderPath != "") {
		glDetachShader(programID, geometryShaderID);
		glDeleteShader(geometryShaderID);
	}
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(fragmentShaderID);
	return programID;
}