/*
* Class: Material
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for a material that may be used by an object.
*/

#include "Material.h"

using namespace Aela;

Texture* Aela::Material::getTexture() {
	return texture;
}

void Aela::Material::setTexture(Texture* texture) {
	this->texture = texture;
}
