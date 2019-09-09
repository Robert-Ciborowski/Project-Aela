/*
* Class: Aela Engine
* Author: Robert Ciborowski
* Date: October 2017
* Description: The main header to the Project Aela library.
*/

#pragma once

// make sure SDL doesn't try to use WinMain
#define SDL_MAIN_HANDLED

// These are headers that are part of Project Aela.
#include "Window/Window.h"
#include "Error Handler/ErrorHandling.h"
#include "Time/Clock.h"
#include "Time/Timer/Timer.h"
#include "Scenes/SceneManager.h"
#include "Resource Management/ResourceManager.h"
#include "2D/Texture/GLTextureLoader.h"
#include "3D/Materials/GLMaterialLoader.h"
#include "3D/Models/OBJLoader.h"
#include "3D/Skybox/GLSkyboxLoader.h"
#include "3D/Maps/Map3DLoader.h"
#include "Audio/WAVEClipLoader.h"
#include "Events/EventHandler.h"
#include "Events/EventConstants.h"
#include "Events/MouseEvent.h"
#include "Audio/AudioPlayer.h"
#include "Animation/Animator.h"
#include "Animation/AnimationLooper.h"
#include "3D/Animation/KeyedAnimator3D.h"
#include "3D/Particles/PlanarParticleEmitter.h"
#include "Menus/Label.h"
#include "User Environment/UserEnvironment.h"
#include "Framerate Calculator/FramerateCalculator.h"
#include "3D/Maps/Map3DExporter.h"
#include "Time/Stopwatch/Stopwatch.h"
#include "Audio/AudioLooper.h"
#include "Python/PythonExecuter.h"
#include "Audio/SimpleAudioPlayer.h"

namespace Aela {
	class Engine {
		public:
			Engine();
			~Engine();

			int setupWindow(unsigned int width, unsigned int height, unsigned int windowXPosition, unsigned int windowYPosition, std::string name);
			int setupRenderer();
			int setupControlManager();
			int setupLUA();
			int setupEventHandler();
			int setupScenes();
			int setupAudio();
			int setupAnimation();
			int setupPythonScripting();
			int loadUserEnvironmentInformation();

			// These functions should get called inside a running loop.
			void update();
			void render();

			bool shouldExit();

			void setUseStopwatch(bool useStopwatch);
			bool isUsingStopwatch();

			void runNextUpdate(std::function<void()> function);

			// These are getters. Some of the Aela Objects inside of this class are not allowed to be accessed externally.
			Window* getWindow();
			// I don't know why, but someone changed getRenderer() to return &, causing many conflicts. I added back the version that returns *.
			GLRenderer& getRendererReference();
			GLRenderer* getRenderer();
			EventHandler* getEventHandler();
			Clock* getClock();
			Timer* getTimer();
			SceneManager* getSceneManager();
			ResourceManager* getResourceManager();
			AudioPlayer* getAudioPlayer();
			AudioLooper* getAudioLooper();
			Animator* getAnimator();
			AnimationLooper* getAnimationLooper();
			UserEnvironment* getUserEnvironment();
			KeyedAnimator* getKeyedAnimator();
			FramerateCalculator* getFramerateCalculator();
			Map3DExporter* getMapExporter();
			Stopwatch* getStopwatch();
			PythonExecuter* getPythonExecuter();
			SimpleAudioPlayer* getSimpleAudioPlayer();

		private:
			// These are global objects who's classes come from Project Aela.
			Window window;
			Clock time;
			Timer timer;
			SceneManager sceneManager;
			ResourceManager resourceManager;
			AudioLooper audioLooper;
			Animator animator;
			AnimationLooper animationLooper;
			UserEnvironment userEnvironment;
			KeyedAnimator keyedAnimator;
			FramerateCalculator framerateCalculator;
			Map3DExporter mapExporter;
			Stopwatch stopwatch;
			PythonExecuter pythonExecuter;
			SimpleAudioPlayer simpleAudioPlayer;

			// These objects have deleted copy constructors. They are stored as pointers
			// so that this class doesn't get its copy constructor deleted automatically.
			// Otherwise, this class would not be exposable to python via boost.
			GLRenderer* renderer = nullptr;
			EventHandler* eventHandler = nullptr;
			AudioPlayer* audioPlayer = nullptr;

			bool useStopwatch = false;

			std::queue<std::function<void()>> functionsToRunNextUpdate;
	};
}
