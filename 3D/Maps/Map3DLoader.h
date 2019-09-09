/*
* Class: Map3D Loader
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to load a map file into a Map3D.
*/

#pragma once

#include "Map3D.h"
#include "../../Resource Management/ResourceLoader.h"
#include "../../Resource Management/ResourceMap.h"
#include "../../XML/XMLLoader.h"
#include "../../Renderer/GLRenderer.h"
#include <unordered_map>

namespace Aela {
	using namespace rapidxml;

	class Map3DLoader : public ResourceLoader, protected XMLLoader {
		public:
			Map3DLoader(std::string resourceRoot);
			virtual ~Map3DLoader();

			bool load(ResourceMap& resources, std::string src) override;
			void bindRenderer(GLRenderer* renderer);

		protected:
			// This function is called upon each XML Node/Tag.
			virtual void processXMLNodeData(Node& node);

		private:
			// TODO make it so that this class does not require a renderer ptr
			GLRenderer* renderer;
			ResourceMap* resources;
			Map3D* map;

			// Because a map refers to models, this loader needs to know what the resource root is.
			// Maybe resource root should be part of ResourceLoader because of this. To be honest,
			// adding a resource root seemed to make everything more complicated.
			std::string resourceRoot;
	};
}