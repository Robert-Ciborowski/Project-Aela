/*
* Class: Exposed Classes
* Author: Robert Ciborowski
* Date: 29/06/2019
* Description: Exposes Aela classes to Python.
*/

#pragma once
#include <boost/python.hpp>

#include "../Aela_Engine.h"
#include "ResourceManagerFunctions.h"
#include "TimerFunctions.h"
#include "KeyFrameMaterialFunctions.h"
#include "../2D/Animation/AnimationTrack2D.h"
#include "../2D/Animation/KeyFrame2D.h"
#include "../3D/Animation/AnimationTrack3D.h"
#include "../3D/Animation/AnimationTrackMaterial.h"
#include "../3D/Animation/AnimationTrackModel.h"
#include "../3D/Animation/KeyFrame3D.h"
#include "../3D/Animation/KeyFrameMaterial.h"
#include "../3D/Animation/KeyFrameModel.h"

using namespace Aela;
using namespace boost::python;

extern Aela::Engine* getEngine();

BOOST_PYTHON_MODULE(Aela) {
	// https://www.boost.org/doc/libs/1_68_0/libs/python/doc/html/tutorial/tutorial/exposing.html
	//class_<Test>("Test", init<std::string>())
	//	// Default constructor: class_<World>("World")
	//	// Another constructor: .def(init<double, double>())
	//	// No constructor: class_<Abstract>("Abstract", no_init)
	//	.def("greet", &Test::getMsg)
	//	.def("set", &Test::setMsg)
	//	;

	// Window

	class_<Window>("Window", no_init)
		.def("getWidth", &Window::getWidth)
		.def("getHeight", &Window::getHeight)
		.def("setDimensions", &Window::setDimensions)
		.def("quit", &Window::quit)
		.def("isFocused", &Window::isFocused)
		.def("isFullscreen", &Window::isFullscreen)
		.def("isMaximized", &Window::isMaximized);

	// Time

	class_<Clock>("Clock", no_init)
		.def("updateTime", &Clock::updateTime)
		.def("getCurrentTimeInNanos", &Clock::getCurrentTimeInNanos)
		.def("getCurrentTimeInMicros", &Clock::getCurrentTimeInMicros)
		.def("getCurrentTimeInMillis", &Clock::getCurrentTimeInMillis)
		.def("getCurrentTimeInSecs", &Clock::getCurrentTimeInSecs)
		.def("getLastFrameTimeInNanos", &Clock::getLastFrameTimeInNanos)
		.def("getLastFrameTimeInMicro", &Clock::getLastFrameTimeInMicro)
		.def("getLastFrameTimeInMillis", &Clock::getLastFrameTimeInMillis)
		.def("getTimeBetweenFramesInNanos", &Clock::getTimeBetweenFramesInNanos)
		.def("getTimeBetweenFramesInMicros", &Clock::getTimeBetweenFramesInMicros)
		.def("getTimeBetweenFramesInMillis", &Clock::getTimeBetweenFramesInMillis);

	class_<Timer>("Timer", no_init)
		.def("scheduleEventInNanos", &PythonFunctions::scheduleEventInNanos)
		.def("scheduleEventInMicros", &PythonFunctions::scheduleEventInMicros)
		.def("scheduleEventInMillis", &PythonFunctions::scheduleEventInMillis)
		.def("scheduleEventInSeconds", &PythonFunctions::scheduleEventInSeconds)
		.def("scheduleEventInNanosUsingTag", &PythonFunctions::scheduleEventInNanosUsingTag)
		.def("scheduleEventInMicrosUsingTag", &PythonFunctions::scheduleEventInMicrosUsingTag)
		.def("scheduleEventInMillisUsingTag", &PythonFunctions::scheduleEventInMillisUsingTag)
		.def("scheduleEventInSecondsUsingTag", &PythonFunctions::scheduleEventInSecondsUsingTag)
		.def("removeEvent", &Timer::removeEvent)
		.def("removeAllEvents", &Timer::removeAllEvents)
		.def("pause", &Timer::pause)
		.def("unpause", &Timer::unpause)
		.def("isPaused", &Timer::isPaused);

	// Resources

	class_<AudioClip>("AudioClip", no_init);
	class_<AudioStream>("AudioStream", no_init);

	class_<Font>("Font", no_init)
		.def("setDefaultSize", &Font::setDefaultSize)
		.def("setAntialiasing", &Font::setAntialiasing);

	class_<Material>("Material", no_init);
	class_<Model>("Model", no_init);
	// class_<Texture>("Texture", no_init);
	// class_<GLTexture, bases<Texture>>("GLTexture", no_init);
	class_<GLTexture>("GLTexture", no_init);

	class_<ResourceManager>("ResourceManager", no_init)
		.def("obtainAudioClip", &PythonFunctions::obtainAudioClip, return_internal_reference<>())
		.def("obtainFont", &PythonFunctions::obtainFont, return_internal_reference<>())
		.def("obtainMaterial", &PythonFunctions::obtainMaterial, return_internal_reference<>())
		.def("obtainModel", &PythonFunctions::obtainModel, return_internal_reference<>())
		.def("obtainGLTexture", &PythonFunctions::obtainGLTexture, return_internal_reference<>())
		.def("loadMaterials", &PythonFunctions::loadMaterials)
		.def("loadOBJModels", &PythonFunctions::loadOBJModels)
		.def("loadPNGTextures", &PythonFunctions::loadPNGTextures)
		.def("loadPNGSpriteSheets", &PythonFunctions::loadPNGSpriteSheets)
		.def("loadParticles", &PythonFunctions::loadParticles)
		.def("loadDDSTextures", &PythonFunctions::loadDDSTextures)
		.def("loadSkyboxes", &PythonFunctions::loadSkyboxes)
		.def("loadMaps", &PythonFunctions::loadMaps)
		.def("loadWaveSoundEffectsUnlessLoaded", &PythonFunctions::loadWaveSoundEffectsUnlessLoaded)
		.def("loadWaveMusicUnlessLoaded", &PythonFunctions::loadWaveMusicUnlessLoaded)
		.def("loadOggSoundEffectsUnlessLoaded", &PythonFunctions::loadOggSoundEffectsUnlessLoaded)
		.def("loadOggMusicUnlessLoaded", &PythonFunctions::loadOggMusicUnlessLoaded)
		.def("loadFonts", &PythonFunctions::loadFonts);

	// Audio
	bool (AudioPlayer:: * isPlayingClip)(AudioClip*) = &AudioPlayer::isPlaying;
	bool (AudioPlayer:: * isPlayingStream)(AudioStream*) = &AudioPlayer::isPlaying;

	class_<AudioPlayer>("AudioPlayer", no_init)
		.def("playClip", &AudioPlayer::playClip)
		.def("stopClip", &AudioPlayer::stopClip)
		.def("playStream", &AudioPlayer::playStream)
		.def("stopStream", &AudioPlayer::stopStream)
		.def("isPlayingClip", isPlayingClip)
		.def("isPlayingStream", isPlayingStream)
		.def("changeVolumeOfPlayingClip", &AudioPlayer::changeVolumeOfPlayingClip)
		.def("changeVolumeOfPlayingStream", &AudioPlayer::changeVolumeOfPlayingStream);

	/*void loopAudioClipUsingSrc(std::string src, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
	void loopAudioStreamUsingSrc(std::string src, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
	void loopAudioClip(AudioClip* clip, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
	void loopAudioClip(AudioStream* stream, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);

	void loopAudioClipInfinitelyUsingSrc(std::string src, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
	void loopAudioStreamInfinitelyUsingSrc(std::string src, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
	void loopAudioClipInfinitely(AudioClip* clip, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
	void loopAudioClipInfinitely(AudioStream* stream, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);

	bool stopClipLoopUsingSrc(std::string src);
	bool stopStreamLoopUsingSrc(std::string src);
	bool stopClipLoop(AudioClip* clip);
	bool stopClipLoop(AudioStream* stream);
	void stopAllLoops();

	bool isLoopingAnyAudio();
	size_t getNumberOfCurrentLoops();*/

	class_<AudioLooper>("AudioLooper", no_init)
		.def("loopAudioClipUsingSrc", &AudioLooper::loopAudioClipUsingSrc)
		.def("loopAudioStreamUsingSrc", &AudioLooper::loopAudioStreamUsingSrc)
		.def("loopAudioClip", &AudioLooper::loopAudioClip)
		.def("loopAudioStream", &AudioLooper::loopAudioStream)
		.def("loopAudioClipInfinitelyUsingSrc", &AudioLooper::loopAudioClipInfinitelyUsingSrc)
		.def("loopAudioStreamInfinitelyUsingSrc", &AudioLooper::loopAudioStreamInfinitelyUsingSrc)
		.def("loopAudioClipInfinitely", &AudioLooper::loopAudioClipInfinitely)
		.def("loopAudioStreamInfinitely", &AudioLooper::loopAudioStreamInfinitely)
		.def("stopClipLoopUsingSrc", &AudioLooper::stopClipLoopUsingSrc)
		.def("stopStreamLoopUsingSrc", &AudioLooper::stopStreamLoopUsingSrc)
		.def("stopClipLoop", &AudioLooper::stopClipLoop)
		.def("stopStreamLoop", &AudioLooper::stopStreamLoop)
		.def("stopAllLoops", &AudioLooper::stopAllLoops)
		.def("isLoopingAnyAudio", &AudioLooper::isLoopingAnyAudio)
		.def("getNumberOfCurrentLoops", &AudioLooper::getNumberOfCurrentLoops)
		.def("getFirstLoopingClip", &AudioLooper::getFirstLoopingClip, return_internal_reference<>());

	// Animation

	class_<Transformable2D>("Transformable2D")
		.def("setTint", &Transformable2D::setTint)
		.def("setX", &Transformable2D::setX)
		.def("setY", &Transformable2D::setY)
		.def("setWidth", &Transformable2D::setWidth)
		.def("setHeight", &Transformable2D::setHeight)
		.def("setWidthHeight", &Transformable2D::setWidthHeight)
		.def("setXY", &Transformable2D::setXY);

	class_<KeyFrame2D>("KeyFrame2D")
		.def("setObject", &KeyFrame2D::setObject)
		.def("setTint", &KeyFrame2D::setTint)
		.def("setObsetDimensionsject", &KeyFrame2D::setDimensions)
		.def("setCurvatureToMovement", &KeyFrame2D::setCurvatureToMovement);

	class_<AnimationTrack2D>("AnimationTrack2D")
		.def("addKeyFrame", &AnimationTrack2D::addKeyFrame)
		.def("addKeyFrameUsingMillis", &AnimationTrack2D::addKeyFrameUsingMillis)
		.def("addKeyFrameUsingSeconds", &AnimationTrack2D::addKeyFrameUsingSeconds)
		.def("setTag", &AnimationTrack2D::setTag);

	void (Transformable3D::* transformable3DSetPosition)(float, float, float) = &Transformable3D::setPosition;
	void (Transformable3D::* transformable3DSetRotation)(float, float, float) = &Transformable3D::setRotation;
	void (Transformable3D::* transformable3DSetScaling)(float, float, float) = &Transformable3D::setScaling;

	class_<Transformable3D>("Transformable3D")
		.def("setPosition", transformable3DSetPosition)
		.def("setRotation", transformable3DSetRotation)
		.def("setScaling", transformable3DSetScaling)
		.def("setWidth", &Transformable2D::setWidth)
		.def("setHeight", &Transformable2D::setHeight)
		.def("setWidthHeight", &Transformable2D::setWidthHeight)
		.def("setXY", &Transformable2D::setXY);

	void (KeyFrame3D::* keyFrame3DSetTranslation)(float, float, float) = &KeyFrame3D::setTranslation;
	void (KeyFrame3D::* keyFrame3DSetRotation)(float, float, float) = &KeyFrame3D::setRotation;
	void (KeyFrame3D::* keyFrame3DSetScaling)(float, float, float) = &KeyFrame3D::setScaling;

	class_<KeyFrame3D>("KeyFrame3D")
		.def("setObject", &KeyFrame3D::setObject)
		.def("setTranslation", keyFrame3DSetTranslation)
		.def("setRotation", keyFrame3DSetRotation)
		.def("setScaling", keyFrame3DSetScaling);

	class_<AnimationTrack3D>("AnimationTrack3D")
		.def("addKeyFrame", &AnimationTrack3D::addKeyFrame)
		.def("addKeyFrameUsingMillis", &AnimationTrack3D::addKeyFrameUsingMillis)
		.def("addKeyFrameUsingSeconds", &AnimationTrack3D::addKeyFrameUsingSeconds)
		.def("setTag", &AnimationTrack3D::setTag);

	class_<KeyFrameMaterial>("KeyFrameMaterial")
		.def("setMaterial", &KeyFrameMaterial::setMaterial)
		.def("setTexture", &PythonFunctions::keyFrameMaterialSetTexture);

	class_<AnimationTrackMaterial>("AnimationTrackMaterial")
		.def("addKeyFrame", &AnimationTrackMaterial::addKeyFrame)
		.def("addKeyFrameUsingMillis", &AnimationTrackMaterial::addKeyFrameUsingMillis)
		.def("addKeyFrameUsingSeconds", &AnimationTrackMaterial::addKeyFrameUsingSeconds)
		.def("setTag", &AnimationTrackMaterial::setTag);

	class_<Animator>("Animator", no_init)
		.def("addAnimationTrack3D", &Animator::addAnimationTrack3D)
		.def("addAnimationTrack2D", &Animator::addAnimationTrack2D)
		.def("delete3DTracksByTag", &Animator::delete3DTracksByTag)
		.def("delete2DTracksByTag", &Animator::delete2DTracksByTag)
		.def("deleteMaterialTracksByTag", &Animator::deleteMaterialTracksByTag)
		.def("deleteTracksByTag", &Animator::deleteTracksByTag)
		.def("trackWithTagExists", &Animator::trackWithTagExists)
		.def("tracksWithTag", &Animator::tracksWithTag);

	void (AnimationLooper::* loop3DAnimation)(AnimationTrack3D*) = &AnimationLooper::loopAnimation;
	void (AnimationLooper::* loop2DAnimation)(AnimationTrack2D*) = &AnimationLooper::loopAnimation;
	void (AnimationLooper::* loopMaterialAnimation)(AnimationTrackMaterial*) = &AnimationLooper::loopAnimation;

	class_<AnimationLooper>("AnimationLooper", no_init)
		.def("loop3DAnimation", loop3DAnimation)
		.def("loop2DAnimation", loop2DAnimation)
		.def("loopMaterialAnimation", loopMaterialAnimation)
		.def("stopLooping3DTrack", &AnimationLooper::stopLooping3DTrack)
		.def("stopLooping2DTrack", &AnimationLooper::stopLooping2DTrack)
		.def("stopLoopingMaterialTrack", &AnimationLooper::stopLoopingMaterialTrack)
		.def("stopLoopingTrack", &AnimationLooper::stopLoopingTrack);

	class_<UserEnvironment>("UserEnvironment", no_init)
		.def("getCurrentAudioDriver", &UserEnvironment::getCurrentAudioDriver)
		.def("getGraphicsVendor", &UserEnvironment::getGraphicsVendor)
		.def("getGraphicsRenderer", &UserEnvironment::getGraphicsRenderer)
		.def("getOpenGLVersion", &UserEnvironment::getOpenGLVersion)
		.def("getGLSLVersion", &UserEnvironment::getGLSLVersion)
		.def("getOpenGLExtensions", &UserEnvironment::getOpenGLExtensions)
		.def("getPlatform", &UserEnvironment::getPlatform)
		.def("getL1CacheLineSize", &UserEnvironment::getL1CacheLineSize)
		.def("getLogicalCPUCores", &UserEnvironment::getLogicalCPUCores)
		.def("getSystemRAM", &UserEnvironment::getSystemRAM)
		.def("getNativeDisplayWidth", &UserEnvironment::getNativeDisplayWidth)
		.def("getNativeDisplayHeight", &UserEnvironment::getNativeDisplayHeight)
		.def("getDisplayDPIDiagonal", &UserEnvironment::getDisplayDPIDiagonal)
		.def("getDisplayDPIWidth", &UserEnvironment::getDisplayDPIWidth)
		.def("getDisplayDPIHeight", &UserEnvironment::getDisplayDPIHeight);

	class_<SimpleAudioPlayer>("SimpleAudioPlayer", no_init)
		.def("addAudioAsClip", &SimpleAudioPlayer::addAudioAsClip)
		.def("addAudioAsStream", &SimpleAudioPlayer::addAudioAsStream)
		.def("playAudio", &SimpleAudioPlayer::playAudio)
		.def("playAudioIfNotPlaying", &SimpleAudioPlayer::playAudioIfNotPlaying)
		.def("isPlayingClip", &SimpleAudioPlayer::isPlayingClip)
		.def("isPlayingStream", &SimpleAudioPlayer::isPlayingStream)
		.def("unloadAudio", &SimpleAudioPlayer::unloadAudio)
		.def("unloadAllClips", &SimpleAudioPlayer::unloadAllClips)
		.def("unloadAllStreams", &SimpleAudioPlayer::unloadAllStreams)
		.def("getClip", &SimpleAudioPlayer::getClip, return_internal_reference<>())
		.def("setClipVolume", &SimpleAudioPlayer::setClipVolume)
		.def("setStreamVolume", &SimpleAudioPlayer::setStreamVolume)
		.def("setAllClipVolumes", &SimpleAudioPlayer::setAllClipVolumes)
		.def("setAllStreamVolumes", &SimpleAudioPlayer::setAllStreamVolumes)
		.def("loopClip", &SimpleAudioPlayer::loopClip)
		.def("loopStream", &SimpleAudioPlayer::loopStream)
		.def("loopClipInfinitely", &SimpleAudioPlayer::loopClipInfinitely)
		.def("loopStreamInfinitely", &SimpleAudioPlayer::loopStreamInfinitely)
		.def("stopClip", &SimpleAudioPlayer::stopClip)
		.def("stopStream", &SimpleAudioPlayer::stopStream)
		.def("stopAllLoops", &SimpleAudioPlayer::stopAllLoops)
		.def("stopEverything", &SimpleAudioPlayer::stopEverything);

	class_<ColourRGB>("ColourRGB")
		.def(init<float, float, float>())
		.def(init<float>())
		.def("setR", &ColourRGB::setR)
		.def("setG", &ColourRGB::setG)
		.def("setB", &ColourRGB::setB)
		.def("getR", &ColourRGB::getR)
		.def("getG", &ColourRGB::getG)
		.def("getB", &ColourRGB::getB);

	class_<ColourRGBA>("ColourRGBA")
		.def(init<float, float, float, float>())
		.def(init<float>())
		.def("setR", &ColourRGBA::setR)
		.def("setG", &ColourRGBA::setG)
		.def("setB", &ColourRGBA::setB)
		.def("setA", &ColourRGBA::setA)
		.def("getR", &ColourRGBA::getR)
		.def("getG", &ColourRGBA::getG)
		.def("getB", &ColourRGBA::getB)
		.def("getA", &ColourRGBA::getA);

	// Engine

	class_<Engine>("Engine", no_init)
		.def("getWindow", &Engine::getWindow, return_internal_reference<>())
		.def("getClock", &Engine::getClock, return_internal_reference<>())
		.def("getTimer", &Engine::getTimer, return_internal_reference<>())
		.def("getResourceManager", &Engine::getResourceManager, return_internal_reference<>())
		.def("getAudioPlayer", &Engine::getAudioPlayer, return_internal_reference<>())
		.def("getAudioLooper", &Engine::getAudioLooper, return_internal_reference<>())
		.def("getAnimator", &Engine::getAnimator, return_internal_reference<>())
		.def("getAnimationLooper", &Engine::getAnimationLooper, return_internal_reference<>())
		.def("getUserEnvironment", &Engine::getUserEnvironment, return_internal_reference<>())
		.def("getSimpleAudioPlayer", &Engine::getSimpleAudioPlayer, return_internal_reference<>());

	def("getEngine", &getEngine, return_value_policy<reference_existing_object>());
}