/*
* Name: ModelEntity
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Aela's GLRenderer to store properties of a 3D entity.
*/

#include "Entity.h"

using namespace Aela;

EntityType Entity::getEntityType() {
	return EntityType::GENERIC;
}

void Aela::Entity::setVisibility(bool visible) {
	this->visible = visible;
}

bool Aela::Entity::isVisible() {
	return visible;
}

void Aela::Entity::toggleVisibility() {
	visible = !visible;
}
