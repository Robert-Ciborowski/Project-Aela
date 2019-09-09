/*
* Class: Project Aela's 3D Animator
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*/

#include "Animator.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace Aela;

// This updates 3D objects based on keyframes given to the Animator. I could split this function to smaller functions
// but its easier to read what is going on when everything is layed out for you. Besides, those functions would be
// called once anyways.
void Animator::update() {
	// TODO: rework this function's 3D stuff so that it isn't organized in a garbage way. Also, optimize it a bit!
	// For help, look at how the 2D stuff is done.

	long long timePassed = time->getTimeBetweenFramesInNanos();

	if (!paused3D) {
		size_t track3DSize = tracks3D.size();
		for (size_t which3DTrack = 0; which3DTrack < track3DSize; which3DTrack++) {
			AnimationTrack3D& track = tracks3D[which3DTrack];
			track.updatePositionInTrack(timePassed);

			auto& firstFramePair = track.getKeyFrames()->at(0);

			KeyFrame3D& keyFrame = firstFramePair.second;
			Transformable3D* object = keyFrame.getObject();

			if (!keyFrame.hasBeenSetUp()) {
				keyFrame.setup();
			}

			long long endTime = firstFramePair.first;
			long long timeSinceKeyFrameStart = track.getPositionInTrack() + 1;

			// Check if this keyframe should have ended by now. If it has, perform the actions necessary.
			if (firstFramePair.first <= track.getPositionInTrack()) {
				if (object != nullptr) {
					// This finds the final scaling of the model.
					if (keyFrame.isUsingScaling()) {
						object->setScaling(*keyFrame.getScaling());
					}

					glm::mat4 pointRotationMatrix = glm::mat4(1);
					glm::vec3* pointRotationValue = nullptr;
					if (keyFrame.isUsingPointRotation()) {
						// This finds the matrix for the point rotation.
						pointRotationValue = keyFrame.getPointRotation()->getRotation();
						pointRotationMatrix *= glm::eulerAngleYXZ(pointRotationValue->y, pointRotationValue->x, pointRotationValue->z);
					}

					// This finds the final position and rotation values and applies them.
					if (keyFrame.isUsingTranslation()) {
						glm::vec3 finalTranslation = *keyFrame.getPosition();
						glm::vec3* originalPosition = keyFrame.getOriginalPosition();
						object->setPosition(*keyFrame.getPosition() + glm::vec3(pointRotationMatrix
							* glm::vec4(*keyFrame.getPointRotation()->getPoint() * glm::vec3(-1), 0)) + *keyFrame.getPointRotation()->getPoint());
						object->timePassedAfterAnimationEnd = track.getPositionInTrack() - firstFramePair.first;
					}

					if (keyFrame.isUsingPointRotation() && keyFrame.isUsingRotation()) {
						object->setRotation(*keyFrame.getRotation() + *pointRotationValue);
					} else if (keyFrame.isUsingRotation()) {
						object->setRotation(*keyFrame.getRotation());
					} else if (keyFrame.isUsingPointRotation()) {
						object->setRotation(*pointRotationValue);
					}
				}

				if (keyFrame.getEndingAction() != nullptr) {
					keyFrame.getEndingAction()();
				}

				// This gets rid of the keyframe.
				if (track.getKeyFrames()->size() != 0) {
					track.getKeyFrames()->erase(track.getKeyFrames()->begin());
				}

				if (track.getKeyFrames()->size() == 0) {
					tracks3D.erase(tracks3D.begin() + which3DTrack);
					which3DTrack--;
					track3DSize--;
					continue;
				}

				track.resetPosition();
				continue;
			}

			// This occurs if the keyframe has not ended yet, and must update the object its linked to accordingly.
			if (object != nullptr) {
				// This figures out a bunch of transformational values and finds the final scaling value to use.
				glm::vec3* originalPosition = keyFrame.getOriginalPosition();
				glm::vec3* originalRotation = keyFrame.getOriginalRotation();
				glm::vec3* originalScaling = keyFrame.getOriginalScaling();
				glm::vec3 newPosition, newRotation;
				if (keyFrame.isUsingTranslation()) {
					glm::vec3* finalTranslation = keyFrame.getPosition();
					newPosition = glm::vec3((((float)finalTranslation->x - originalPosition->x) / endTime) * timeSinceKeyFrameStart,
						(((float)finalTranslation->y - originalPosition->y) / endTime) * timeSinceKeyFrameStart,
						(((float)finalTranslation->z - originalPosition->z) / endTime) * timeSinceKeyFrameStart);
				}
				if (keyFrame.isUsingRotation()) {
					glm::vec3* finalRotation = keyFrame.getRotation();
					newRotation = glm::vec3((((float)finalRotation->x - originalRotation->x) / endTime) * timeSinceKeyFrameStart,
						(((float)finalRotation->y - originalRotation->y) / endTime) * timeSinceKeyFrameStart,
						(((float)finalRotation->z - originalRotation->z) / endTime) * timeSinceKeyFrameStart);
				}
				if (keyFrame.isUsingScaling()) {
					glm::vec3* finalScaling = keyFrame.getScaling();
					glm::vec3 newScaling(originalScaling->x + (((float)finalScaling->x - originalScaling->x) / endTime) * timeSinceKeyFrameStart,
						originalScaling->y + (((float)finalScaling->y - originalScaling->y) / endTime) * timeSinceKeyFrameStart,
						originalScaling->z + (((float)finalScaling->z - originalScaling->z) / endTime) * timeSinceKeyFrameStart);
					object->setScaling(newScaling);
				}

				// This finds the matrix for the point rotation.
				glm::mat4 pointRotationMatrix = glm::mat4(1);
				glm::vec3* pointRotationRotation = keyFrame.getPointRotation()->getRotation();
				if (keyFrame.isUsingPointRotation()) {
					pointRotationMatrix *= glm::eulerAngleYXZ(pointRotationRotation->y / endTime * timeSinceKeyFrameStart,
						pointRotationRotation->x / endTime * timeSinceKeyFrameStart, pointRotationRotation->z / endTime * timeSinceKeyFrameStart);
				}

				// This finds the final position values and applies them.
				if (keyFrame.isUsingTranslation() && keyFrame.isUsingPointRotation()) {
					object->setPosition(*originalPosition + newPosition + glm::vec3(pointRotationMatrix
						* glm::vec4(*keyFrame.getPointRotation()->getPoint() * glm::vec3(-1), 0)) + *keyFrame.getPointRotation()->getPoint());
				} else if (keyFrame.isUsingTranslation()) {
					object->setPosition(*originalPosition + newPosition);
				}

				// This finds the final rotation values and applies them.
				if (keyFrame.isUsingRotation() && keyFrame.isUsingPointRotation()) {
					object->setRotation(*originalRotation + newRotation + *pointRotationRotation * glm::vec3((float)timeSinceKeyFrameStart / endTime));
				} else if (keyFrame.isUsingRotation()) {
					object->setRotation(*originalRotation + newRotation);
				} else if (keyFrame.isUsingPointRotation()) {
					object->setRotation(*pointRotationRotation * glm::vec3((float)timeSinceKeyFrameStart / endTime));
				}
			}
		}

		size_t trackMaterialSize = tracksMaterial.size();
		for (size_t whichMaterialTrack = 0; whichMaterialTrack < trackMaterialSize; whichMaterialTrack++) {
			AnimationTrackMaterial& track = tracksMaterial[whichMaterialTrack];
			track.updatePositionInTrack(timePassed);

			if (track.getKeyFrames()->size() == 0) {
				continue;
			}

			auto& firstFramePair = track.getKeyFrames()->at(0);

			KeyFrameMaterial& keyFrame = firstFramePair.second;
			Material* material = keyFrame.getMaterial();

			if (!keyFrame.hasBeenSetUp()) {
				keyFrame.setup();
			}

			// Check if this keyframe should have ended by now. If it has, perform the actions necessary.
			if (firstFramePair.first <= track.getPositionInTrack()) {
				if (material != nullptr) {
					material->setTexture(keyFrame.getTexture());
				}

				if (keyFrame.getEndingAction() != nullptr) {
					keyFrame.getEndingAction()();
				}

				if (track.getKeyFrames()->size() != 0) {
					track.getKeyFrames()->erase(track.getKeyFrames()->begin());
				}

				if (track.getKeyFrames()->size() == 0) {
					tracksMaterial.erase(tracksMaterial.begin() + whichMaterialTrack);
					whichMaterialTrack--;
					trackMaterialSize--;
					continue;
				}

				track.resetPosition();
				continue;
			}
		}

		size_t trackModelSize = tracksModel.size();
		for (size_t whichModelTrack = 0; whichModelTrack < trackModelSize; whichModelTrack++) {
			AnimationTrackModel& track = tracksModel[whichModelTrack];
			track.updatePositionInTrack(timePassed);

			if (track.getKeyFrames()->size() == 0) {
				continue;
			}

			auto& firstFramePair = track.getKeyFrames()->at(0);

			KeyFrameModel& keyFrame = firstFramePair.second;
			ModelEntity* modelEntity = keyFrame.getModelEntity();

			if (!keyFrame.hasBeenSetUp()) {
				keyFrame.setup();
			}

			// Check if this keyframe should have ended by now. If it has, perform the actions necessary.
			if (firstFramePair.first <= track.getPositionInTrack()) {
				if (modelEntity != nullptr) {
					modelEntity->setModel(keyFrame.getModel());
				}

				if (keyFrame.getEndingAction() != nullptr) {
					keyFrame.getEndingAction()();
				}

				if (track.getKeyFrames()->size() != 0) {
					track.getKeyFrames()->erase(track.getKeyFrames()->begin());
				}

				if (track.getKeyFrames()->size() == 0) {
					tracksModel.erase(tracksModel.begin() + whichModelTrack);
					whichModelTrack--;
					trackModelSize--;
					continue;
				}

				track.resetPosition();
				continue;
			}
		}
	}

	if (!paused2D) {
		size_t track2DSize = tracks2D.size();
		for (size_t which2DTrack = 0; which2DTrack < track2DSize; which2DTrack++) {
			AnimationTrack2D& track = tracks2D[which2DTrack];
			track.updatePositionInTrack(timePassed);

			if (track.getKeyFrames()->size() == 0) {
				continue;
			}

			auto& firstFramePair = track.getKeyFrames()->at(0);

			KeyFrame2D& keyFrame = firstFramePair.second;
			std::shared_ptr<Transformable2D> object = keyFrame.getObject();

			if (!keyFrame.hasBeenSetUp()) {
				keyFrame.setup();
			}

			// Check if this keyframe should have ended by now. If it has, perform the actions necessary.
			if (firstFramePair.first <= track.getPositionInTrack()) {
				keyFrame.end();

				if (track.getKeyFrames()->size() != 0) {
					track.getKeyFrames()->erase(track.getKeyFrames()->begin());
				}

				if (track.getKeyFrames()->size() == 0) {
					tracks2D.erase(tracks2D.begin() + which2DTrack);
					which2DTrack--;
					track2DSize--;
					continue;
				}

				track.resetPosition();
				continue;
			}

			// This occurs if the keyframe has not ended yet, and must update the object its linked to accordingly.
			if (object != nullptr) {
				long long endTime = firstFramePair.first;
				long long timeSinceKeyFrameStart = track.getPositionInTrack() + 1;

				if (keyFrame.isUsingTint()) {
					object->setTint(keyFrame.getTintAtTime(timeSinceKeyFrameStart, endTime));
				}

				if (keyFrame.isUsingDimensions()) {
					object->setDimensions(keyFrame.getDimensionsAtTime(timeSinceKeyFrameStart, endTime));
				}
			}
		}
	}
}

void Animator::setTime(Clock* time) {
	this->time = time;
}

void Animator::addAnimationTrack3D(AnimationTrack3D* track) {
	if (track->getKeyFrames()->size() != 0) {
		tracks3D.push_back(*track);
	}
}

AnimationTrack3D* Animator::get3DTrack(std::string name) {
	for (auto& track : tracks3D) {
		if (track.getTag() == name) {
			return &track;
		}
	}
	return nullptr;
}

void Animator::addAnimationTrack2D(AnimationTrack2D* track) {
	if (track->getKeyFrames()->size() != 0) {
		tracks2D.push_back(*track);
	}
}

AnimationTrack2D* Animator::get2DTrack(std::string name) {
	for (auto& track : tracks2D) {
		if (track.getTag() == name) {
			return &track;
		}
	}
	return nullptr;
}

void Aela::Animator::addAnimationTrackMaterial(AnimationTrackMaterial* track) {
	if (track->getKeyFrames()->size() != 0) {
		tracksMaterial.push_back(*track);
	}
}

AnimationTrackMaterial* Aela::Animator::getMaterialTrack(std::string name) {
	for (auto& track : tracksMaterial) {
		if (track.getTag() == name) {
			return &track;
		}
	}
	return nullptr;
}

void Aela::Animator::addAnimationTrackModel(AnimationTrackModel* track) {
	if (track->getKeyFrames()->size() != 0) {
		tracksModel.push_back(*track);
	}
}

AnimationTrackModel* Aela::Animator::getModelTrack(const std::string name) {
	for (auto& track : tracksModel) {
		if (track.getTag() == name) {
			return &track;
		}
	}
	return nullptr;
}

int Animator::delete3DTracksByTag(const std::string tag) {
	int counter = 0;
	for (unsigned int i = 0; i < tracks3D.size(); i++) {
		if (tracks3D[i].getTag() == tag) {
			counter++;
			tracks3D.erase(tracks3D.begin() + i);
			i--;
		}
	}
	return counter;
}

int Aela::Animator::deleteModelTracksByTag(const std::string tag) {
	int counter = 0;
	for (unsigned int i = 0; i < tracksModel.size(); i++) {
		if (tracksModel[i].getTag() == tag) {
			counter++;
			tracksModel.erase(tracksModel.begin() + i);
			i--;
		}
	}
	return counter;
}

int Aela::Animator::deleteMaterialTracksByTag(const std::string tag) {
	int counter = 0;
	for (unsigned int i = 0; i < tracksMaterial.size(); i++) {
		if (tracksMaterial[i].getTag() == tag) {
			counter++;
			tracksMaterial.erase(tracksMaterial.begin() + i);
			i--;
		}
	}
	return counter;
}

int Animator::delete2DTracksByTag(const std::string tag) {
	int counter = 0;
	for (unsigned int i = 0; i < tracks2D.size(); i++) {
		if (tracks2D[i].getTag() == tag) {
			counter++;
			tracks2D.erase(tracks2D.begin() + i);
			i--;
		}
	}
	return counter;
}

int Animator::deleteTracksByTag(const std::string tag) {
	int counter = 0;
	counter += delete3DTracksByTag(tag);
	counter += delete2DTracksByTag(tag);
	counter += deleteMaterialTracksByTag(tag);
	counter += deleteModelTracksByTag(tag);
	return counter;
}

bool Animator::trackWithTagExists(const std::string tag) {
	for (auto& track : tracks3D) {
		if (track.getTag() == tag) {
			return true;
		}
	}

	for (auto& track : tracks2D) {
		if (track.getTag() == tag) {
			return true;
		}
	}

	for (auto& track : tracksMaterial) {
		if (track.getTag() == tag) {
			return true;
		}
	}

	for (auto& track : tracksModel) {
		if (track.getTag() == tag) {
			return true;
		}
	}
	return false;
}

long long Animator::tracksWithTag(const std::string tag) {
	long long counter = 0;

	for (auto& track : tracks3D) {
		if (track.getTag() == tag) {
			counter++;
		}
	}

	for (auto& track : tracks2D) {
		if (track.getTag() == tag) {
			counter++;
		}
	}

	for (auto& track : tracksMaterial) {
		if (track.getTag() == tag) {
			counter++;
		}
	}

	for (auto& track : tracksModel) {
		if (track.getTag() == tag) {
			counter++;
		}
	}

	return counter;
}

void Aela::Animator::pause3DAnimations() {
	paused3D = true;
}

void Aela::Animator::unpause3DAnimations() {
	paused3D = false;
}

bool Aela::Animator::is3DPaused() {
	return paused3D;
}

void Aela::Animator::pause2DAnimations() {
	paused2D = true;
}

void Aela::Animator::unpause2DAnimations() {
	paused2D = false;
}

bool Aela::Animator::is2DPaused() {
	return paused2D;
}
