/*
* Class: Menu
* Author: Robert Ciborowski
* Date: 20/03/2017
* Description: Represents an entire menu in our game. It can contain various
               containers, widgets and other assorted GUI elements.
*/

#pragma once
#include "Container.h"
#include "../Events/Event.h"
#include "../Events/EventConstants.h"
#include "../Events/EventListenerList.h"
#include "../Events/EventListener.h"
#include "../2D/Simple 2D Framebuffer/Simple2DGLFramebuffer.h"
#include "../Utilities/Rect/Rect.h"

namespace Aela {
	class Menu : public Container {
		public:
			Menu();
			virtual ~Menu();

			void init(Rect<int>* renderDimensions, GLRenderer& renderer);
			void init(Rect<int>* renderDimensions, GLRenderer& renderer, int x, int y);

			virtual void render(GLRenderer& renderer);

			bool isInitialized();

		protected:
			bool initialized = false;

		private:
			Simple2DGLFramebuffer buffer;
	};
}
