/*
* Class: Game Animator
* Author: Robert Ciborowski
* Date: 04/03/2018
* Description: A class that uses Project Aela's Animator to loop animations.
*/

#include "AnimationLooper.h"

void Aela::AnimationLooper::update() {
	for (auto pair : track3Ds) {
		if (animator->get3DTrack(pair.second.getTag()) == nullptr) {
			animator->addAnimationTrack3D(&pair.second);
		}
	}

	for (auto pair : track2Ds) {
		if (animator->get2DTrack(pair.second.getTag()) == nullptr) {
			animator->addAnimationTrack2D(&pair.second);
		}
	}

	for (auto pair : trackMaterials) {
		if (animator->getMaterialTrack(pair.second.getTag()) == nullptr) {
			animator->addAnimationTrackMaterial(&pair.second);
		}
	}

	for (auto pair : trackModels) {
		if (animator->getModelTrack(pair.second.getTag()) == nullptr) {
			animator->addAnimationTrackModel(&pair.second);
		}
	}
}

void Aela::AnimationLooper::loopAnimation(AnimationTrack3D* track) {
	track3Ds[track->getTag()] = *track;
}

void Aela::AnimationLooper::loopAnimation(AnimationTrack2D* track) {
	track2Ds[track->getTag()] = *track;
}

void Aela::AnimationLooper::loopAnimation(AnimationTrackMaterial* track) {
	trackMaterials[track->getTag()] = *track;
}

void Aela::AnimationLooper::loopAnimation(AnimationTrackModel* track) {
	trackModels[track->getTag()] = *track;
}

void Aela::AnimationLooper::stopLooping3DTrack(const std::string tag) {
	auto iter = track3Ds.find(tag);
	if (iter != track3Ds.end()) {
		track3Ds.erase(iter);
	}
}

void Aela::AnimationLooper::stopLooping2DTrack(const std::string tag) {
	auto iter = track2Ds.find(tag);
	if (iter != track2Ds.end()) {
		track2Ds.erase(iter);
	}
}

void Aela::AnimationLooper::stopLoopingMaterialTrack(const std::string tag) {
	auto iter = trackMaterials.find(tag);
	if (iter != trackMaterials.end()) {
		trackMaterials.erase(iter);
	}
}

void Aela::AnimationLooper::stopLoopingModelTrack(const std::string tag) {
	auto iter = trackModels.find(tag);
	if (iter != trackModels.end()) {
		trackModels.erase(iter);
	}
}

void Aela::AnimationLooper::stopLoopingTrack(const std::string tag) {
	stopLooping3DTrack(tag);
	stopLooping2DTrack(tag);
	stopLoopingMaterialTrack(tag);
}

void Aela::AnimationLooper::setAnimator(Animator* animator) {
	this->animator = animator;
}
