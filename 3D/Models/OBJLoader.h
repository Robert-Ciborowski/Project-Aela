/*
* Class: OBJ Loader
* Author: Robert Ciborowski
* Date: 04/07/2017
* Description: A class for loading OBJ files into models.
*/

#pragma once

#include "Model.h"
#include "../../Resource Management/ResourceLoader.h"
#include "../../Resource Management/ResourceMap.h"

#include <map>

namespace Aela {
	class OBJLoader : public ResourceLoader {
		public:
			OBJLoader();
			virtual ~OBJLoader();

			virtual bool load(ResourceMap& resources, std::string src);

		private:
			struct VertexPacket {
				glm::vec3 position;
				glm::vec2 uv;
				glm::vec3 normal;

				bool operator<(const VertexPacket that) const {
					return memcmp((void*)this, (void*) &that, sizeof(VertexPacket))>0;
				};
			};

			void setupSubModel(SubModel* subModel, std::vector<unsigned int>* vertexIndices, std::vector<unsigned int>* uvIndices,
				std::vector<unsigned int>* normalIndices, std::vector<glm::vec3>* tempVertices, std::vector<glm::vec2>* tempUVs,
				std::vector<glm::vec3>* tempNormals);

			void indexVBO(std::vector<glm::vec3>* inputVertices, std::vector<glm::vec2>* inputUVs, std::vector<glm::vec3>* inputNormals,
				std::vector<unsigned short>* outputIndices, std::vector<glm::vec3>* outputVertices, std::vector<glm::vec2>* outputUVs,
				std::vector<glm::vec3>* outputNormals);

			bool getSimilarVertex(OBJLoader::VertexPacket* data, std::map<VertexPacket, unsigned short>* vertexDataMap, unsigned short* result);
	};
}