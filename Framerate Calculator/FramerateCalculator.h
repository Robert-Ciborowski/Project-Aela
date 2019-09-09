/*
* Class: Framerate Calculator
* Author: Robert Ciborowski
* Date: 18/08/2017
* Description: A class used to keep track of framerate.
*/

#pragma once
#include <chrono>

namespace Aela {
	class FramerateCalculator {
		public:
			FramerateCalculator() {

			}

			void calculate(long long currentTimeInNanos, long long timeBetweenFrames);

			// These are getters and setters.
			double getTrueFPS(), getSmoothedFPS(), getFPSSmoothingAmount();
			void setFPSSmoothing(double fpsSmoothingAmount), setTimeBetweenFrameChecks(long long timeBetweenFrameChecks);

		private:
			double trueFPS = -1, smoothedFPS = -1, fpsSmoothingAmount = 0.2f;
			long long timeBetweenFrameChecks = 250;
			long long timeOfLastFrameCheck = 0;
	};
}