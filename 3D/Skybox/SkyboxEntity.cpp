/*
* Class: SkyboxEntity
* Author: Robert Ciborowski
* Date: 12/08/2017
* Description: A class used by Aela's GLRenderer to store properties of a skybox entity.
*              Keep in mind that a skybox entity can move and animate, making it behave
*              like other entities.
*/

#include "SkyboxEntity.h"

using namespace Aela;

void SkyboxEntity::setSkybox(Skybox* skybox) {
	this->skybox = skybox;
}

Skybox* SkyboxEntity::getSkybox() {
	return skybox;
}

EntityType SkyboxEntity::getEntityType() {
	return EntityType::SKYBOX;
}
