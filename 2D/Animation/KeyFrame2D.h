/*
* Class: Project Aela's 2D Key Frame
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used by Aela's animator to perform transformations on 2D objects.
*/

#pragma once

#include <memory>

#include "../../Animation/KeyFrame.h"
#include "../Transformable/Transformable2D.h"

namespace Aela {
	class KeyFrame2D : public KeyFrame {
		public:
			KeyFrame2D() : tint(1, 1, 1, 1), dimensions(0, 0, 0, 0) {}

			virtual void setup();
			virtual void setup(KeyFrame* previousKeyFrame);
			virtual void end();

			ColourRGBA getTintAtTime(long long currentTime, long long endTime);
			Rect<int> getDimensionsAtTime(long long currentTime, long long endTime);

			// I have to make a copy of this function. Otherwise, this awful compiler breaks.
			Rect<int> getDimensionsAtTime2(long long currentTime, long long endTime);

			KeyFrameType getType();
			void setObject(std::shared_ptr<Transformable2D> object);
			std::shared_ptr<Transformable2D> getObject();
			void setTint(ColourRGBA* tint);
			ColourRGBA* getTint();
			void setOriginalTint(ColourRGBA* originalTint);
			ColourRGBA* getOriginalTint();
			void setDimensions(Rect<int>* dimensions);
			Rect<int>* getDimensions();
			Rect<int>* getOriginalDimensions();
			bool isUsingDimensions();
			bool isUsingTint();
			void setCurvatureToMovement(float curvatureMultiplier);
			bool isUsingCurvature();
			glm::vec2 getCurvatureOffset(float ratioOfTimeTaken);

		private:
			std::shared_ptr<Transformable2D> object = nullptr;
			ColourRGBA tint, originalTint;
			Rect<int> dimensions, originalDimensions;

			bool useDimensions = false;
			bool useTint = false;

			// These are precalculated and used internally if the object is to move along a
			// simple, elliptical curve when going to its destination.
			bool useCurvature = false;
			float curvatureMultiplier = 0.0f;
			float sinA, cosA, mSquared, d, r, rSquared, curvatureSign;

			void setupCurvatureVariables();
			float curvatureFunction(float linearDistanceTravelled);
	};
}