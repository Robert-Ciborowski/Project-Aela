/*
* Class: Model
* Author: Robert Ciborowski
* Date: 04/07/2017
* Description: Represents an object resource, which is part of a model and entity.
*/

#pragma once

#include "../../Resource Management/Resource.h"
#include "SubModel.h"

#include <GLM/glm.hpp>
#include <vector>
#include <unordered_map>

namespace Aela {
	class Model : public Resource {
		public:
			Model(std::string src);
			virtual ~Model();

			std::vector<SubModel>* getSubModels();
			void setSubModels(std::vector<SubModel>* subModels);
			size_t getNumberOfVertices();
			size_t getNumberOfUVs();
			size_t getNumberOfNormals();
			size_t getNumberOfIndices();

		private:
			std::vector<SubModel> subModels;

			size_t numberOfVertices = 0;
			size_t numberOfUVs = 0;
			size_t numberOfNormals = 0;
			size_t numberOfIndices = 0;
	};
}