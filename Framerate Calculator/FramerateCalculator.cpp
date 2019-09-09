/*
* Class: Framerate Calculator
* Author: Robert Ciborowski
* Date: 18/08/2017
* Description: A class used to keep track of framerate.
*/

#include "FramerateCalculator.h"

using namespace Aela;

void FramerateCalculator::calculate(long long currentTimeInNanos, long long timeBetweenFrames) {
	// This does some simple math for framerate calculating.
	if ((currentTimeInNanos - timeOfLastFrameCheck) >= timeBetweenFrameChecks) {
		if (trueFPS == -1 && timeBetweenFrames != 0) {
			trueFPS = 1000000000.0 / timeBetweenFrames;
			smoothedFPS = trueFPS;
		} else if (timeBetweenFrames != 0) {
			trueFPS = 1000000000.0 / timeBetweenFrames;
			smoothedFPS = (smoothedFPS * fpsSmoothingAmount) + ((1000000000.0 / timeBetweenFrames) * (1.0 - fpsSmoothingAmount));
			timeOfLastFrameCheck = currentTimeInNanos;
		} else {
			// timeBetweenFrames = 0? Whoa, your computer is THAT fast? If you're really that rich, buy me a new PC!
			trueFPS = 1000;
			smoothedFPS = (smoothedFPS * fpsSmoothingAmount) + (1000.0 * (1.0 - fpsSmoothingAmount));
			timeOfLastFrameCheck = currentTimeInNanos;
		}
	}
}

double FramerateCalculator::getTrueFPS() {
	return trueFPS;
}

double FramerateCalculator::getSmoothedFPS() {
	return smoothedFPS;
}

double FramerateCalculator::getFPSSmoothingAmount() {
	return fpsSmoothingAmount;
}

void FramerateCalculator::setFPSSmoothing(double fpsSmoothingAmount) {
	this->fpsSmoothingAmount = fpsSmoothingAmount;
}

void FramerateCalculator::setTimeBetweenFrameChecks(long long timeBetweenFrameChecks) {
	this->timeBetweenFrameChecks = timeBetweenFrameChecks;
}