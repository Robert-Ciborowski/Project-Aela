/*
* Class: OGG Stream Loader
* Author: Robert Ciborowski
* Date: 11/07/2019
* Description: Loads .ogg files as audio streams.
*/

#pragma once

#include "../Resource Management/ResourceLoader.h"
#include "../Resource Management/ResourceMap.h"

namespace Aela {
	class OGGStreamLoader : public ResourceLoader {
	public:
		OGGStreamLoader();
		virtual ~OGGStreamLoader();

		virtual bool load(ResourceMap& resources, std::string src);
	};
}