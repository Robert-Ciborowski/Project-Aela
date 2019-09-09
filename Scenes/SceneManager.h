/**
 * Class: SceneManager
 * Author: Robert Ciborowski
 * Date: 26/02/2017
 * Description: Keeps track of the scene system.
 *
 * NOTE: This class has a non-virtual destructor! Do not modify this class to inherit from it.
 */

#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "../Events/Event.h"
#include "../Events/EventListenerList.h"

#include <unordered_map>

namespace Aela {
	class SceneManager final {
		public:
			SceneManager();
			~SceneManager();

			int init(EventHandler& eventHandler);

			void registerScene(Aela::Scene* scene, unsigned int id);

			void update();
			void render(GLRenderer& renderer);

			void setCurrentScene(unsigned int id);
			unsigned int getCurrentSceneId();
			Scene* getCurrentScene();

			void setDisposingScenesOnDestroy(bool disposingScenesOnDestroy);
			bool isDisposingScenesOnDestroy();
		private:
			std::unordered_map<unsigned int, Aela::Scene*> scenes;

			Scene* currentScene = nullptr;
			unsigned int currentSceneId = 0;
			EventListenerList* listeners = nullptr;

			Scene* previousScene = nullptr;
			bool sceneChangeRequested = false;

			bool disposingScenesOnDestroy = true;

			/**
			 * Why does this need to be private? Well, keep in mind that avoiding getters is necessary whenever possible,
			 * as it can increase coupling and allow objects to try to implement shape that is not theirs. Knowing this,
			 * we can reduce coupling by preventing outside classes from knowing that we even use a Scene* at all, beyond
			 * those that do initialization. This is why the getCurrentSceneId() function exists; even though the Scene class
			 * has a getId() function, we can avoid exposing unnecessary instances of Scene to other classes by delegating rather
			 * than getting. In addition this allows us to follow the "one dot per line" principle (see Discord #links channel)
			 * by writing sceneManager->getCurrentSceneId() rather than sceneManger->getCurrentScene()->getId() (the getCurrentScene
			 * function was removed outright).
			 * 
			 * So why should we not just remove this function outright? First of all, it IS used inside of THIS class, so we require its
			 * implementation to stick around, and it is most useable as a member of SceneManager. In addition, should the case arise
			 * where we actually NEED access to a Scene instance, we can make this function public. However, it is better to try to avoid
			 * at all costs needing to access a Scene instance, as the idea behind having a scene manager is the "set it and forget it"
			 * mentality. This is where the user should be able to initialize a scene and NEVER have to interact with that instance again.
			 * All state changes and updates should happen ONLY through event listeners and callbacks.
			 * 
			 * (Whew, that was long.)
			 */
			Scene* getScene(unsigned int id);

			void consumeSceneChangeEvent();

			void handleMousePressed(Event* event);
			void handleMouseReleased(Event* event);
			void handleMouseMoved(Event* event);
		};
}
