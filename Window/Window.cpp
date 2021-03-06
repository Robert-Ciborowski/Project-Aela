/*
* Name: Project Aela's Window Class
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class that uses SDL to form a window.
*/

#include "Window.h"
#include "../Error Handler/ErrorHandling.h"
#include <GL/glew.h>
#include <iostream>
using namespace Aela;

void Window::addProperty(WindowFlag flag) {
	bool flagExists = false;

	for (unsigned int i = 0; i < flags.size(); i++) {
		if (flags[i] == flag) {
			flagExists = true;
			AelaErrorHandling::windowError("Flag already exists.");
			break;
		}
	}

	if (!flagExists) {
		flags.push_back(flag);
	}
}

bool Window::createWindow(int width, int height, int xPosition, int yPosition, std::string name) {
	// This sets general object properties.
	setBasicProperties(width, height, name);
	SDL_Init(SDL_INIT_EVERYTHING);
	Uint32 sdlFlags = generateWindowFlagsForSDL();
	

	// This creates and sets up the SDL window.
	window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, sdlFlags);
	
	if (window == NULL) {
		return false;
	}

	SDL_SetWindowSize(window, width, height);
	SDL_SetWindowPosition(window, xPosition, yPosition);
	return true;
}

bool Aela::Window::createWindowAtCenter(int width, int height, std::string name) {
	// This sets general object properties.
	setBasicProperties(width, height, name);
	SDL_Init(SDL_INIT_EVERYTHING);
	Uint32 sdlFlags = generateWindowFlagsForSDL();


	// This creates and sets up the SDL window.
	window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdlFlags);

	if (window == NULL) {
		return false;
	}

	SDL_SetWindowSize(window, width, height);
	return true;
}

void Window::getDimensions(int* widthVariable, int* heightVariable) {
	*widthVariable = width;
	*heightVariable = height;
}

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}

Rect<unsigned int>* Window::getDimensions() {
	return &dimensions;
}

void Window::getPosition(int* xPositionVariable, int* yPositionVariable) {
	SDL_GetWindowPosition(window, xPositionVariable, yPositionVariable);
}

bool Window::makeWindowOpenGLContext() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
 //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	openGLContext = SDL_GL_CreateContext(window);

	if (openGLContext == NULL) {
		AelaErrorHandling::windowError("Window", "Could not create an Open GL context! Your computer might be too old to support OpenGL 4.");
		return false;
	}
	
	return true;
}

void Window::updateBuffer() {
	SDL_GL_SwapWindow(window);
}

void Aela::Window::setDimensions(int width, int height) {
	SDL_SetWindowSize(window, width, height);
	this->width = width;
	this->height = height;
	dimensions = Rect<unsigned int>(0, 0, width, height);
	recentlyResized = true;
}

void Window::getCursorPositionInWindow(int* x, int* y) {
	SDL_GetMouseState(x, y);
}

void Window::getCursorPositionGlobally(int* x, int* y) {
	SDL_GetGlobalMouseState(x, y);
}

void Window::setCursorPositionInWindow(int x, int y) {
	SDL_WarpMouseInWindow(window, x, y);
}

void Window::setCursorPositionGlobally(int x, int y) {
	SDL_WarpMouseGlobal(x, y);
}

void Aela::Window::setFullscreen(WindowFlag type) {
	switch (type) {
		case WindowFlag::AELA_WINDOW_FULLSCREEN:
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			fullscreen = true;
			break;
		case WindowFlag::AELA_WINDOW_FULLSCREEN_DESKTOP:
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			fullscreen = true;
			break;
		default:
			SDL_SetWindowFullscreen(window, 0);
			fullscreen = false;
	}
}

bool Aela::Window::isFullscreen() {
	return fullscreen;
}

void Aela::Window::show() {
	SDL_ShowWindow(window);
}

std::string Window::getWindowName() {
	return windowName;
}

void Window::quit() {
	shouldQuit = true;
}

bool Window::quitCheck() {
	return shouldQuit;
}

void Window::showCursor() {
	SDL_ShowCursor(SDL_ENABLE);
}

void Window::hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void Window::gainFocus() {
	SDL_RaiseWindow(window);
}

Uint32 Aela::Window::generateWindowFlagsForSDL() {
	// The window's SDL flags. By default, this uses SDL_WINDOW_MOUSE_FOCUS as if it were NULL.
	SDL_WindowFlags resizableFlag = SDL_WINDOW_MOUSE_FOCUS, shownFlag = SDL_WINDOW_SHOWN,
		borderlessFlag = SDL_WINDOW_MOUSE_FOCUS, minimizedFlag = SDL_WINDOW_MOUSE_FOCUS,
		openGLFlag = SDL_WINDOW_MOUSE_FOCUS, highDPIFlag = SDL_WINDOW_ALLOW_HIGHDPI;

	// This starts looking at the flags inputted as parameters.
	for (unsigned int i = 0; i < flags.size(); i++) {
		if (flags[i] == WindowFlag::AELA_WINDOW_RESIZABLE) {
			resizableFlag = SDL_WINDOW_RESIZABLE;
		} else if (flags[i] == WindowFlag::AELA_WINDOW_HIDDEN) {
			shownFlag = SDL_WINDOW_HIDDEN;
		} else if (flags[i] == WindowFlag::AELA_WINDOW_BORDERLESS) {
			borderlessFlag = SDL_WINDOW_BORDERLESS;
		} else if (flags[i] == WindowFlag::AELA_WINDOW_MINIMIZED) {
			minimizedFlag = SDL_WINDOW_MINIMIZED;
		} else if (flags[i] == WindowFlag::AELA_WINDOW_OPENGL) {
			openGLFlag = SDL_WINDOW_OPENGL;
		} else if (flags[i] == WindowFlag::AELA_WINDOW_HIGH_DPI) {
			highDPIFlag = SDL_WINDOW_ALLOW_HIGHDPI;
		} else if (flags[i] == WindowFlag::AELA_WINDOW_FULLSCREEN || flags[i] == WindowFlag::AELA_WINDOW_FULLSCREEN_DESKTOP
			|| flags[i] == WindowFlag::AELA_WINDOW_WINDOWED) {
			setFullscreen(flags[i]);
		}
	}

	Uint32 flags = resizableFlag | shownFlag | borderlessFlag | minimizedFlag | openGLFlag | highDPIFlag;
	return flags;
}

void Aela::Window::setBasicProperties(int width, int height, std::string name) {
	windowName = name.c_str();
	this->width = width;
	this->height = height;
	hasFocus = true;
	maximized = true;
	dimensions = Rect<unsigned int>(0, 0, width, height);
}

bool Window::isFocused() {
	return hasFocus;
}

bool Aela::Window::isMaximized() {
	return maximized;
}

void Aela::Window::processSDLWindowEvent(SDL_Event& event) {
	switch (event.window.event) {
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			hasFocus = true;
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			hasFocus = false;
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			maximized = false;
			std::cout << "Minimized!\n";
			break;
		case SDL_WINDOWEVENT_RESTORED:
			maximized = true;
			std::cout << "Maximized!\n";
			break;
	}
}

bool Aela::Window::wasResized() {
	if (recentlyResized) {
		recentlyResized = false;
		return true;
	}
	return false;
}
