/*
* Class: Model
* Author: Robert Ciborowski
* Date: 04/07/2017
* Description: Represents a model resource used in the application.
*/

#include "Model.h"
#include "../../Error Handler/ErrorHandling.h"
#include <iostream>

using namespace Aela;

Aela::Model::Model(std::string src) : Resource(src) {
}

Aela::Model::~Model() {
}

std::vector<SubModel>* Aela::Model::getSubModels() {
	return &subModels;
}

void Aela::Model::setSubModels(std::vector<SubModel>* subModels) {
	this->subModels = *subModels;

	for (SubModel& subModel : this->subModels) {
		numberOfVertices += subModel.getVertexSize();
		numberOfUVs += subModel.getUVSize();
		numberOfNormals += subModel.getNormalSize();
		numberOfIndices += subModel.getIndexSize();
	}
}

size_t Aela::Model::getNumberOfVertices() {
	return numberOfVertices;
}

size_t Aela::Model::getNumberOfUVs() {
	return numberOfUVs;
}

size_t Aela::Model::getNumberOfNormals() {
	return numberOfNormals;
}

size_t Aela::Model::getNumberOfIndices() {
	return numberOfIndices;
}
