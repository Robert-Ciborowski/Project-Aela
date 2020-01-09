/*
* Class: Scene
* Author: Robert Ciborowski
* Date: 26/02/2017
* Description: A class which represents a scene in the game.
*/

#pragma once
#include <vector>
#include "../3D/Maps/Map3D.h"
#include "../Renderer/GLRenderer.h"
#include "../Menus/Menu.h"
#include "../Time/Stopwatch/Stopwatch.h"

namespace Aela {
	class Scene {
		friend class SceneManager;

		public:
			Scene();
			~Scene();

			void show();
			void hide();

			void update();
			void render(GLRenderer& renderer);

			void enableMenu(Rect<unsigned int>* renderDimensions, GLRenderer& renderer);
			void enableMenu(Rect<unsigned int>* renderDimensions, GLRenderer& renderer, int x, int y);

			// These are getters and setters.
			Menu* getMenu();
			void setMap(Map3D* map);
			Map3D* getMap();
			void setActiveSkybox(unsigned int activeSkybox);
			unsigned int getActiveSkybox();

			void putParticleEmitter(ParticleEmitter* emitter);
			void clearParticleEmitters();

			// REMOVE THIS!!
			Stopwatch* stopwatch;
		private:
			Map3D* map = nullptr;
			std::vector<ParticleEmitter*> particleEmitters;

			// This keeps track of the skybox that is currently in use.
			unsigned int activeSkybox = 0;

			Menu menu;

			MouseEvent mousePressed, mouseReleased, mouseMoved;
			bool processMousePressed = false, processMouseReleased = false, processMouseMoved = false;

			void handleMousePressed(Event* event);
			void handleMouseReleased(Event* event);
			void handleMouseMoved(Event* event);
	};
}
