/*
* Class: OGG Clip Loader
* Author: Robert Ciborowski
* Date: 11/07/2019
* Description: Loads .ogg files as audio clips.
*/

#pragma once
#include "../Resource Management/ResourceLoader.h"
#include "../Resource Management/ResourceMap.h"
#include <map>

namespace Aela {
	class OGGClipLoader : public ResourceLoader {
	public:
		OGGClipLoader();
		virtual ~OGGClipLoader();

		virtual bool load(ResourceMap& resources, std::string src);

	};
}