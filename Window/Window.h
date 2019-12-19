/*
* Name: Project Aela's Window Class
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class that uses SDL to form a window.
*/

#pragma once
#include <string>
#include <vector>
#include "../Utilities/Rect/Rect.h"
#include "SDL.h"

namespace Aela {
	enum class WindowFlag {
		AELA_WINDOW_RESIZABLE, AELA_WINDOW_NON_RESIZABLE,
		AELA_WINDOW_HIDDEN, AELA_WINDOW_BORDERLESS,
		AELA_WINDOW_MINIMIZED, AELA_WINDOW_MAXIMIZED,
		AELA_WINDOW_OPENGL, AELA_WINDOW_FULLSCREEN,
		AELA_WINDOW_FULLSCREEN_DESKTOP, AELA_WINDOW_WINDOWED ,
		AELA_WINDOW_HIGH_DPI
	};

	class Window {
		public:
			// These are the constructors.
			Window() {
				windowName = "Aela Window";
				width = 0;
				height = 0;
			}

			// These are the behaviours.
			void addProperty(WindowFlag flag);
			bool createWindow(int setWidth, int setHeight, int setXPosition, int setYPosition, std::string setName);
			void getDimensions(int* widthVariable, int* heightVariable);
			int getWidth();
			int getHeight();
			Rect<unsigned int>* getDimensions();
			void getPosition(int* xPositionVariable, int* yPositionVariable);
			bool makeWindowOpenGLContext();
			void updateBuffer();
			void setDimensions(int width, int height);
			void getCursorPositionInWindow(int* x, int* y);
			void getCursorPositionGlobally(int* x, int* y);
			void setCursorPositionInWindow(int x, int y);
			void setCursorPositionGlobally(int x, int y);
			void setFullscreen(WindowFlag type);
			void show();
			std::string getWindowName();
			void quit();
			bool quitCheck();
			void processSDLWindowEvent(SDL_Event& event);
			bool wasResized();

			bool isFullscreen();
			bool isFocused();
			bool isMaximized();

			void showCursor();
			static void hideCursor();

	private:
		// These are typical window properties.
		int width, height;
		std::string windowName;
		bool resizable = false, visibility = false, borderless = false, maximized = false, taskbarVisibility = false,
			hasFocus = false, shouldQuit = false, fullscreen = false, recentlyResized = false;

		// This is the SDL_Window used by this class.
		SDL_Window* window;

		// These are the enumerators for the window flags.
		std::vector<WindowFlag> flags;

		// This is used to bind SDL with OpenGL.
		SDL_GLContext openGLContext;

		Rect<unsigned int> dimensions;
	};
}