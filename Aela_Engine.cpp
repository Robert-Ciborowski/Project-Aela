/*
 * Name: Project Aela - Main
 * Author: Robert Ciborowski
 * Date: October 2016
 * Description: Project Aela's main.cpp file.
*/

#include "Aela_Engine.h"

// This includes standard headers.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <mutex>
#include <signal.h>

using namespace Aela;

Aela::Engine::Engine() : resourceManager(0), timer(&time) {
	// This is crucial for many error handling-related features.
	// AelaErrorHandling::handleSignal(SIGSEGV);
}

Aela::Engine::~Engine() {
	delete renderer;
	delete eventHandler;
}

int Aela::Engine::setupWindow(unsigned int width, unsigned int height, unsigned int windowXPosition, unsigned int windowYPosition, std::string name) {
	setupWindowProperties();
	bool windowCreationSuccess = window.createWindow(width, height, windowXPosition, windowYPosition, name);

	if (!windowCreationSuccess) {
		AelaErrorHandling::windowError("Project Aela Window", "The Aela Window failed to initialise!");
		return -1;
	}

	window.makeWindowOpenGLContext();
	return 0;
}

int Aela::Engine::setupWindowAtCenter(unsigned int width, unsigned int height, std::string name) {
	setupWindowProperties();
	bool windowCreationSuccess = window.createWindowAtCenter(width, height, name);

	if (!windowCreationSuccess) {
		AelaErrorHandling::windowError("Project Aela Window", "The Aela Window failed to initialise!");
		return -1;
	}

	window.makeWindowOpenGLContext();
	return 0;
}

int Aela::Engine::setupRenderer() {
	// This initializes GLEW.
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		return -1;
	}

	// This passes the window and time manager to the renderer and control manager.
	// Please note that the window must be set before calling start functions.
	renderer = new GLRenderer();
	renderer->setWindow(&window);
	renderer->setTime(&time);
	renderer->setup3D();
	renderer->setup2D();
	renderer->setupWindow();
	return 0;
}

int Aela::Engine::setupControlManager() {
	// This sets the Control Manager up and tells it to prevent the camera from being inverted.
	return 0;
}

int Aela::Engine::setupLUA() {
	// This was here just as a test but it breaks.
	/* luaManager.exposeObject(&lol, "lol");
	LuaScript testScript;
	testScript.loadScript("res/scripts/test.lua");
	testScript.callFunction("testFunction");*/
	return 0;
}

#include <functional>

#if defined __MINGW32__ || defined __APPLE__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "err_ovl_no_viable_oper"
int Aela::Engine::setupEventHandler() {
	eventHandler = new EventHandler();
    eventHandler->bindWindow(&window);
    eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(Camera3D::onEvent, renderer.getCamera()));
    eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(Camera3D::onEvent, renderer.getCamera()));
    eventHandler->addListener(EventConstants::WINDOW_RESIZE, bindListener(Renderer::onEvent, &renderer));
    renderer->setEventHandler(&eventHandler);
	eventHandler->bindRenderer(&renderer);
	eventHandler->bindWindow(&window);
    eventHandler->start();
    return 0;
}
#pragma clang diagnostic pop
#endif

#if defined (_MSC_VER)
int Aela::Engine::setupEventHandler() {
	eventHandler = new EventHandler();
    eventHandler->bindWindow(&window);
    eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(Camera3D::onEvent, renderer->getCamera()));
    eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(Camera3D::onEvent, renderer->getCamera()));
    eventHandler->addListener(EventConstants::WINDOW_RESIZE, bindListener(Renderer::onEvent, renderer));
    renderer->setEventHandler(eventHandler);
	eventHandler->bindRenderer(renderer);
	eventHandler->bindWindow(&window);
    eventHandler->start();
    return 0;
}
#endif

int Engine::setupScenes() {
	return sceneManager.init(*eventHandler);
}

int Aela::Engine::setupAudio() {
	audioPlayer = new AudioPlayer();

	if (!audioPlayer->init()) {
		return -1;
	}

	audioLooper.setAudioPlayer(audioPlayer);
	audioLooper.setClock(&time);
	audioLooper.setResourceManager(&resourceManager);
	simpleAudioPlayer.setResourceManager(&resourceManager);
	simpleAudioPlayer.setAudioPlayer(audioPlayer);
	simpleAudioPlayer.setAudioLooper(&audioLooper);
	return 0;
}

int Aela::Engine::setupAnimation() {
    animator.setTime(&time);
    animationLooper.setAnimator(&animator);
    keyedAnimator.setTime(&time);
    keyedAnimator.setWindow(&window);
    
#if defined (_MSC_VER)
    eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(KeyedAnimator::onEvent, &keyedAnimator));
    eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(KeyedAnimator::onEvent, &keyedAnimator));
#endif
    
#if defined __MINGW32__ || defined __APPLE__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "err_ovl_no_viable_oper"
    eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(KeyedAnimator::onEvent, &keyedAnimator));
    eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(KeyedAnimator::onEvent, &keyedAnimator));
#endif
    return 0;
}

int Aela::Engine::setupPythonScripting() {
	return pythonExecuter.startAllowingImports(*this);
}

int Aela::Engine::loadUserEnvironmentInformation() {
	if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0) {
		AelaErrorHandling::windowError("Aela Engine", "SDL was not initialized before determining user environment information.");
		return 1;
	}

	userEnvironment.generateInfo(renderer);
	return 0;
}

// This method is meant to be run by another program that uses the Project Aela library. It starts Project Aela.
void Engine::update() {
	if (useStopwatch) {
		stopwatch.startRecording("Event Handler Updating");
		// Note: Events should be updated first.
		eventHandler->updateSDLEvents();
		stopwatch.stopRecording("Event Handler Updating");

		stopwatch.startRecording("Time & Timer Updating");
		time.updateTime();
		timer.update();
		stopwatch.stopRecording("Time & Timer Updating");

		stopwatch.startRecording("Scene Manager Updating");
		sceneManager.update();
		stopwatch.stopRecording("Scene Manager Updating");

		stopwatch.startRecording("Animation Updating");
		animationLooper.update();
		animator.update();
		keyedAnimator.update();
		stopwatch.stopRecording("Animation Updating");

		stopwatch.startRecording("Audio Looper Updating");
		audioLooper.update();
		stopwatch.stopRecording("Audio Looper Updating");

		while (!functionsToRunNextUpdate.empty()) {
			if (functionsToRunNextUpdate.front() != nullptr) {
				functionsToRunNextUpdate.front()();
			}
			functionsToRunNextUpdate.pop();
		}
	} else {
		// Note: Events should be updated first.
		eventHandler->updateSDLEvents();
		time.updateTime();
		timer.update();
		sceneManager.update();
		animationLooper.update();
		animator.update();
		keyedAnimator.update();
		audioLooper.update();

		while (!functionsToRunNextUpdate.empty()) {
			functionsToRunNextUpdate.front()();
			functionsToRunNextUpdate.pop();
		}
	}

	// if (!window.isMaximized()) {
	// 	SDL_Delay(100);
	// }
}

void Engine::render() {
	sceneManager.getCurrentScene()->stopwatch = &stopwatch;
	// stopwatch.startRecording("Scene Manager Rendering");
	sceneManager.render(*renderer);
	// stopwatch.stopRecording("Scene Manager Rendering");
}

bool Engine::shouldExit() {
	return window.quitCheck() || AelaErrorHandling::programCloseWasRequested();
}

void Aela::Engine::setUseStopwatch(bool useStopwatch) {
	this->useStopwatch = useStopwatch;
}

bool Aela::Engine::isUsingStopwatch() {
	return useStopwatch;
}

Window* Engine::getWindow() {
	return &window;
}

GLRenderer& Engine::getRendererReference() {
	return *renderer;
}

GLRenderer* Aela::Engine::getRenderer() {
	return renderer;
}

EventHandler* Engine::getEventHandler() {
	return eventHandler;
}

Clock* Engine::getClock() {
	return &time;
}

Timer* Aela::Engine::getTimer() {
	return &timer;
}

SceneManager* Engine::getSceneManager() {
	return &sceneManager;
}

ResourceManager* Engine::getResourceManager() {
	return &resourceManager;
}

AudioPlayer* Engine::getAudioPlayer() {
	return audioPlayer;
}

AudioLooper* Engine::getAudioLooper() {
	return &audioLooper;
}

Animator* Engine::getAnimator() {
	return &animator;
}

AnimationLooper* Aela::Engine::getAnimationLooper() {
	return &animationLooper;
}

UserEnvironment* Engine::getUserEnvironment() {
	return &userEnvironment;
}

KeyedAnimator* Engine::getKeyedAnimator() {
	return &keyedAnimator;
}

FramerateCalculator* Engine::getFramerateCalculator() {
	return &framerateCalculator;
}

Map3DExporter* Aela::Engine::getMapExporter() {
	return &mapExporter;
}

Stopwatch* Aela::Engine::getStopwatch() {
	return &stopwatch;
}

PythonExecuter* Aela::Engine::getPythonExecuter() {
	return &pythonExecuter;
}

SimpleAudioPlayer* Aela::Engine::getSimpleAudioPlayer() {
	return &simpleAudioPlayer;
}

void Aela::Engine::setupWindowProperties() {
	// Note: the renderer will render black and then set the window to be shown rather than hidden once the window is bound with it.
	window.addProperty(WindowFlag::AELA_WINDOW_OPENGL);
	window.addProperty(WindowFlag::AELA_WINDOW_HIGH_DPI);
	// window.addProperty(WindowFlag::AELA_WINDOW_BORDERLESS);
}

void Engine::runNextUpdate(std::function<void()> function) {
	functionsToRunNextUpdate.push(function);
}
