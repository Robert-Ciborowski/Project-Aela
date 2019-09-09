#include "UserEnvironment.h"

void Aela::UserEnvironment::generateInfo(GLRenderer* renderer) {
	graphicsVendor = renderer->getInformation(GLRendererInformation::VENDOR);
	graphicsRenderer = renderer->getInformation(GLRendererInformation::RENDERER);
	openGLVersion = renderer->getInformation(GLRendererInformation::OPENGL_VERSION);
	GLSLVersion = renderer->getInformation(GLRendererInformation::GLSL_VERSION);
//    openGLExtensions = renderer->getInformation(GLRendererInformation::OPENGL_EXTENSIONS);

	for (int i = 0; i < SDL_GetNumAudioDevices(0); i++) {
		audioDeviceNames.push_back(std::string(SDL_GetAudioDeviceName(i, 0)));
	}

	for (int i = 0; i < SDL_GetNumAudioDrivers(); i++) {
		audioDeviceDrivers.push_back(std::string(SDL_GetAudioDriver(i)));
	}

	platform = std::string(SDL_GetPlatform());
	l1CacheLineSize = SDL_GetCPUCacheLineSize();
	logicalCPUCores = SDL_GetCPUCount();
	systemRAM = SDL_GetSystemRAM();
}

std::vector<std::string>* Aela::UserEnvironment::getAudioDeviceNames() {
	return &audioDeviceNames;
}

std::vector<std::string>* Aela::UserEnvironment::getAudioDeviceDrivers() {
	return &audioDeviceDrivers;
}

std::string Aela::UserEnvironment::getCurrentAudioDriver() {
	return currentAudioDriver;
}

std::string Aela::UserEnvironment::getGraphicsVendor() {
	return graphicsVendor;
}

std::string Aela::UserEnvironment::getGraphicsRenderer() {
	return graphicsRenderer;
}

std::string Aela::UserEnvironment::getOpenGLVersion() {
	return openGLVersion;
}

std::string Aela::UserEnvironment::getGLSLVersion() {
	return GLSLVersion;
}

std::string Aela::UserEnvironment::getOpenGLExtensions() {
	return openGLExtensions;
}

std::string Aela::UserEnvironment::getPlatform() {
	return platform;
}

unsigned int Aela::UserEnvironment::getL1CacheLineSize() {
	return l1CacheLineSize;
}

unsigned int Aela::UserEnvironment::getLogicalCPUCores() {
	return logicalCPUCores;
}

unsigned int Aela::UserEnvironment::getSystemRAM() {
	return systemRAM;
}

void UserEnvironment::getNativeDisplayResolution(int displayID, int* width, int* height) {
	SDL_DisplayMode mode;
	if (SDL_GetCurrentDisplayMode(displayID, &mode) == 0) {
		*width = mode.w;
		*height = mode.h;
	} else {
		*width = 0;
		*height = 0;
	}
}

int Aela::UserEnvironment::getNativeDisplayWidth(int displayID) {
	SDL_DisplayMode mode;
	if (SDL_GetCurrentDisplayMode(displayID, &mode) == 0) {
		return mode.w;
	}
	else {
		return 0;
	}
}

int Aela::UserEnvironment::getNativeDisplayHeight(int displayID) {
	SDL_DisplayMode mode;
	if (SDL_GetCurrentDisplayMode(displayID, &mode) == 0) {
		return mode.h;
	}
	else {
		return 0;
	}
}

void Aela::UserEnvironment::getDisplayDPI(int displayID, float* diagonalDPI, float* horizontalDPI, float* verticalDPI) {
	SDL_GetDisplayDPI(displayID, diagonalDPI, horizontalDPI, verticalDPI);
}

float Aela::UserEnvironment::getDisplayDPIDiagonal(int displayID) {
	float returnValue;
	SDL_GetDisplayDPI(displayID, &returnValue, nullptr, nullptr);
	return returnValue;
}

float Aela::UserEnvironment::getDisplayDPIWidth(int displayID) {
	float returnValue;
	SDL_GetDisplayDPI(displayID, nullptr, &returnValue, nullptr);
	return returnValue;
}

float Aela::UserEnvironment::getDisplayDPIHeight(int displayID) {
	float returnValue;
	SDL_GetDisplayDPI(displayID, nullptr, nullptr, &returnValue);
	return returnValue;
}
