#include "stdafx.h"
#include "SceneManager.h"

using namespace Aela;

// public members

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
	if (disposingScenesOnDestroy) {
		for (auto ptr : scenes) {
			delete ptr.second;
		}
	}
}

#if defined __MINGW32__ || defined __APPLE__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "err_ovl_no_viable_oper"
int Aela::SceneManager::init(EventHandler& eventHandler) {
    //eventHandler.addListener(EventConstants::KEY_PRESSED, bindListener(SceneManager::onEvent, this));
    //eventHandler.addListener(EventConstants::KEY_RELEASED, bindListener(SceneManager::onEvent, this));
    eventHandler.addListener(EventConstants::MOUSE_PRESSED, bindListener(SceneManager::handleMousePressed, this));
    eventHandler.addListener(EventConstants::MOUSE_RELEASED, bindListener(SceneManager::handleMouseReleased, this));
    eventHandler.addListener(EventConstants::MOUSE_MOTIONED, bindListener(SceneManager::handleMouseMoved, this));
    return 0;
}
#endif

#if defined (_MSC_VER)
int Aela::SceneManager::init(EventHandler& eventHandler) {
    //eventHandler.addListener(EventConstants::KEY_PRESSED, bindListener(SceneManager::onEvent, this));
    //eventHandler.addListener(EventConstants::KEY_RELEASED, bindListener(SceneManager::onEvent, this));
    eventHandler.addListener(EventConstants::MOUSE_PRESSED, bindListener(SceneManager::handleMousePressed, this));
    eventHandler.addListener(EventConstants::MOUSE_RELEASED, bindListener(SceneManager::handleMouseReleased, this));
    eventHandler.addListener(EventConstants::MOUSE_MOTIONED, bindListener(SceneManager::handleMouseMoved, this));
    return 0;
}
#endif


void SceneManager::registerScene(Scene* scene, unsigned int id) {
	scenes.emplace(id, scene);
}

void Aela::SceneManager::update() {
	// determine the current scene
	consumeSceneChangeEvent();

	// update the current scene
	if (currentScene != nullptr) {
		currentScene->update();
	}
}

void Aela::SceneManager::render(GLRenderer& renderer) {
	// render the current scene
	if (currentScene != nullptr) {
		currentScene->render(renderer);
	}
}

void SceneManager::setCurrentScene(unsigned int id) {
	if (currentScene != nullptr) {
		previousScene = currentScene;
	}

	currentScene = getScene(id);
	currentSceneId = id;
	sceneChangeRequested = true;
}

unsigned int SceneManager::getCurrentSceneId() {
	return currentSceneId;
}

Scene* Aela::SceneManager::getCurrentScene() {
	return currentScene;
}

void SceneManager::setDisposingScenesOnDestroy(bool disposingScenesOnDestroy) {
	this->disposingScenesOnDestroy = disposingScenesOnDestroy;
}

bool SceneManager::isDisposingScenesOnDestroy() {
	return disposingScenesOnDestroy;
}

// private members

Scene* SceneManager::getScene(unsigned int id) {
	// determine the requested scene
	auto item = scenes.find(id);
	if (item == scenes.end()) {
		return nullptr;
	}
	return item->second;
}

void SceneManager::consumeSceneChangeEvent() {
	if (sceneChangeRequested) {
		if (previousScene != nullptr) {
			// hide the previous scene
			previousScene->hide();
			previousScene = nullptr;
		}

		if (currentScene != nullptr) {
			// show the current scene
			currentScene->show();
		}

		sceneChangeRequested = false;
	}
}

void SceneManager::handleMousePressed(Event* event) {
	// send the event to the current scene
	if (currentScene != nullptr) {
		currentScene->handleMousePressed(event);
	}
}

void SceneManager::handleMouseReleased(Event* event) {
	// send the event to the current scene
	if (currentScene != nullptr) {
		currentScene->handleMouseReleased(event);
	}
}

void SceneManager::handleMouseMoved(Event* event) {
	// send the event to the current scene
	if (currentScene != nullptr) {
		currentScene->handleMouseMoved(event);
	}
}
