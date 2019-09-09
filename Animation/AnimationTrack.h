/*
* Class: Project Aela's Animation Track
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class that stores 2D keyframes.
*/

#pragma once
#include <vector>
#include <utility>
#include <functional>
#include <iostream>
#include "../Time/Clock.h"

namespace Aela {
	template <class T> class AnimationTrack {
		friend class Animator;
		public:
			AnimationTrack() {}

			void updatePositionInTrack(long long timeToAdd) {
				positionInTrack += timeToAdd;
			}

			long long getPositionInTrack() {
				return positionInTrack;
			}

			unsigned long long getPositionInFirstFrameAtTime(unsigned long long timeInNanos) {
				Clock clock;
				return timeInNanos - clock.getCurrentTimeInNanos() + positionInTrack;
			}

			void getKeyFrameInfoAtTime(long long nanoSecondsAway, long long* timeUntilKeyFrame, T** keyFrame, size_t* keyFramePosition) {
				if (keyFrames.empty()) {
					*timeUntilKeyFrame = 0;
					*keyFrame = nullptr;
					*keyFramePosition = 0;
					return;
				}

				long long addedTime = 0;
				for (size_t i = 0; i < keyFrames.size(); i++) {
					auto pair = keyFrames[i];

					if (pair.first + addedTime - positionInTrack > nanoSecondsAway) {
						*timeUntilKeyFrame = pair.first + addedTime - positionInTrack;
						*keyFrame = &pair.second;
						*keyFramePosition = i;
						return;
					}

					addedTime += pair.first;
				}

				*keyFramePosition = keyFrames.size() - 1;
				auto& pair = keyFrames[*keyFramePosition];
				*timeUntilKeyFrame = pair.first + addedTime - positionInTrack;
				*keyFrame = &pair.second;
			}

			bool frameIsFirst(T* frame) {
				if (keyFrames.empty()) {
					return false;
				}

				return &keyFrames[0].second == frame;
			}

			long long getTimeUntilKeyFrame(T* frame) {
				long long addedTime = 0;

				for (auto& pair : keyFrames) {
					if (&pair.second == frame) {
						return pair.first + addedTime - positionInTrack;
					}

					addedTime += pair.first;
				}

				return 0;
			}

			bool addKeyFrame(long long time, T* keyFrame) {
				if (time <= positionInTrack) {
					// Nice try, but your time has passed.
					return false;
				}

				if (!keyFrames.empty()) {
					keyFrame->setup(&keyFrames[keyFrames.size() - 1].second);
				}

				/*for (auto& pair : keyFrames) {
					if (pair.first > time) {
						keyFrames.push_back(std::pair<long long, T>(time, *keyFrame));
						return true;
					}
				}*/

				keyFrames.push_back(std::pair<long long, T>(time, *keyFrame));
				return true;
			}

			bool addKeyFrameUsingMillis(long long timeInMillis, T* keyFrame) {
				return addKeyFrame(timeInMillis * 1000000, keyFrame);
			}

			bool addKeyFrameUsingSeconds(long long timeInSeconds, T* keyFrame) {
				return addKeyFrame(timeInSeconds * 1000000000, keyFrame);
			}

			std::vector<std::pair<long long, T>>* getKeyFrames() {
				return &keyFrames;
			}

			void setTag(std::string tag) {
				this->tag = tag;
			}

			std::string getTag() {
				return tag;
			}

		protected:
			// This specifies the time that this key frame list should fire after the previous one.
			long long positionInTrack = 0;

			// This is the list of KeyFrames.
			std::vector<std::pair<long long, T>> keyFrames;

			// This is used in order to let the Animator perform actions upon this list. For example, if something tells the Animator
			// to delete all tracks with the tag "camera_animation" in order to get rid of all camera animations and this list has that
			// exact tag, then this list will be deleted. This is also used by a track observer to see which track is calling its
			// on-state-change functions.
			std::string tag = "";

		private:
			void resetPosition() {
				positionInTrack = 0;
			}
	};
}
