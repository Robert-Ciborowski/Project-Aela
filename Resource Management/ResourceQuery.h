/*
* Class: ResourceQuery
* Author: Robert Ciborowski
* Date: 24/04/2017
* Description: Represents query used to load a resource used in the application.
*/

#pragma once

#include "stdafx.h"
#include "ResourceLoader.h"

namespace Aela {
	class ResourceQuery {
		public:
			ResourceQuery(std::string src, bool crucial, ResourceLoader* loader);
			virtual ~ResourceQuery();

			std::string getSrc();
			bool isCrucial();
			ResourceLoader* getLoader();
		private:
			std::string src;
			bool crucial;
			ResourceLoader* loader;
	};
}