/*
* Class: Project Aela's Key Frame
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used for keyframing with the animator.
*/

#include "KeyFrame.h"
#include <iostream>
using namespace Aela;

void KeyFrame::setup() {
	wasSetUp = true;
}

void KeyFrame::setup(KeyFrame* previousKeyFrame) {
	wasSetUp = true;
}

void KeyFrame::end() {
	if (endingAction != nullptr) {
		endingAction();
	}
}

bool KeyFrame::hasBeenSetUp() {
	return wasSetUp;
}

void KeyFrame::setEndingAction(std::function<void()> endingAction) {
	this->endingAction = endingAction;
}

std::function<void()> KeyFrame::getEndingAction() {
	return endingAction;
}