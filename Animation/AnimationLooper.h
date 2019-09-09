/*
* Class: Game Animator
* Author: Robert Ciborowski
* Date: 04/03/2018
* Description: A class that uses Project Aela's Animator to loop animations.
*/

#pragma once
#include <unordered_map>
#include "Animator.h"
#include "../3D/Animation/AnimationTrack3D.h"
#include "../2D/Animation/AnimationTrack2D.h"
#include "../3D/Animation/AnimationTrackMaterial.h"

namespace Aela {
	class AnimationLooper {
		public:
			AnimationLooper() {}

			void update();

			void loopAnimation(AnimationTrack3D* track);
			void loopAnimation(AnimationTrack2D* track);
			void loopAnimation(AnimationTrackMaterial* track);
			void loopAnimation(AnimationTrackModel* track);
			void stopLooping3DTrack(const std::string tag);
			void stopLooping2DTrack(const std::string tag);
			void stopLoopingMaterialTrack(const std::string tag);
			void stopLoopingModelTrack(const std::string tag);
			void stopLoopingTrack(const std::string tag);

			void setAnimator(Animator* animator);
		private:
			Animator* animator = nullptr;

			std::unordered_map<std::string, AnimationTrack3D> track3Ds;
			std::unordered_map<std::string, AnimationTrack2D> track2Ds;
			std::unordered_map<std::string, AnimationTrackMaterial> trackMaterials;
			std::unordered_map<std::string, AnimationTrackModel> trackModels;
	};
}