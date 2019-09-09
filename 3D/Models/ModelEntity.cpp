/*
* Name: ModelEntity
* Author: Robert Ciborowski
* Date: 06/08/2017
* Description: A class used by Aela's GLRenderer to store properties of a 3D entity.
*              Note: This class used to be known as "Model" but was changed.
*/

#include "ModelEntity.h"
#include "../../Error Handler/ErrorHandling.h"

using namespace Aela;

Model* Aela::ModelEntity::getModel() {
	return model;
}

void Aela::ModelEntity::setModel(Model* model) {
	this->model = model;
}

BoundingBox3D* Aela::ModelEntity::getBoundingBox() {
	return &boundingBox;
}

EntityType ModelEntity::getEntityType() {
	return EntityType::MODEL;
}

// This generates a bounding box by taking the greatest and smallest values for x, and and z of the model. Keep in mind that an
// entity's bounding box's position most likely won't be at (0, 0, 0) relative to the model. To see why, consider a model in which
// the vertex with the greatest x position has an x position of 3 while the vertex with the smallest x position has an x position
// of -6. The bounding box's own x position (which represents where the center of the bounding box is) will be -1.5.
void Aela::ModelEntity::generateBoundingBox() {
	if (model != nullptr && model->getSubModels() != nullptr && model->getSubModels()->size() != 0) {
		SubModel* firstSubModel = &model->getSubModels()->at(0);
		float smallestX = firstSubModel->getVertices()->at(firstSubModel->getIndices()->at(0)).x, greatestX = smallestX,
			smallestY = firstSubModel->getVertices()->at(firstSubModel->getIndices()->at(0)).y, greatestY = smallestY,
			smallestZ = firstSubModel->getVertices()->at(firstSubModel->getIndices()->at(0)).z, greatestZ = smallestZ;

		for (SubModel subModel : *model->getSubModels()) {
			for (unsigned short index : *subModel.getIndices()) {
				glm::vec3* vertex = &subModel.getVertices()->at(index);
				if (vertex->x < smallestX) {
					smallestX = vertex->x;
				} else if (vertex->x > greatestX) {
					greatestX = vertex->x;
				}
				if (vertex->y < smallestY) {
					smallestY = vertex->y;
				} else if (vertex->y > greatestY) {
					greatestY = vertex->y;
				}
				if (vertex->z < smallestZ) {
					smallestZ = vertex->z;
				} else if (vertex->z > greatestZ) {
					greatestZ = vertex->z;
				}
			}
		}

		boundingBox.setValues(position.x - ((greatestX - smallestX) / 2 - greatestX), position.y - ((greatestY - smallestY) / 2 - greatestY), position.z - ((greatestZ - smallestZ) / 2 - greatestZ), greatestX - smallestX, greatestY - smallestY, greatestZ - smallestZ);
		boundingBox.generateVertices();
	} else {
		AelaErrorHandling::consoleWindowError("ModelEntity", "Something tried to generate an entity's bounding box without loading its baseModel first!");
	}
}
