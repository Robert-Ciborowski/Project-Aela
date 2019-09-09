/*
* Class: Project Aela's Model Key Frame
* Author: Robert Ciborowski
* Date: 04/03/2018
* Description: A class used by Aela's animator to switch the model used by a ModelEntity.
*/

#include "KeyFrameModel.h"

using namespace Aela;

KeyFrameType Aela::KeyFrameModel::getType() {
	return KeyFrameType::MODEL;
}

void Aela::KeyFrameModel::setup() {
	KeyFrame::setup();
}

void KeyFrameModel::setup(KeyFrame* previousKeyFrame) {
	KeyFrame::setup(previousKeyFrame);
}

void Aela::KeyFrameModel::setModelEntity(ModelEntity* modelEntity) {
	this->modelEntity = modelEntity;
}

void Aela::KeyFrameModel::setModel(Model* model) {
	this->model = model;
}

ModelEntity* Aela::KeyFrameModel::getModelEntity() {
	return modelEntity;
}

Model* Aela::KeyFrameModel::getModel() {
	return model;
}

void Aela::KeyFrameModel::setUseModel(bool use) {
	useModel = use;
}
