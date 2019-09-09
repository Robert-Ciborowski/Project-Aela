/*
* Class: Resource Functions
* Author: Robert Ciborowski
* Date: 02/07/2019
* Description: ResourceManager binding to use for Python scripts.
			   See the bottom for the actual binding part itself.
*/

#pragma once
#include <boost/python.hpp>
#include "../3D/Animation/KeyFrameMaterial.h"

namespace PythonFunctions {
	using namespace Aela;

	void keyFrameMaterialSetTexture(KeyFrameMaterial& self, GLTexture* texture) {
		self.setTexture(texture);
	}
}