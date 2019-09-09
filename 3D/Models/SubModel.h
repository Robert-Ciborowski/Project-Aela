/*
* Class: SubModel
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for an object, which represents part of a model. It uses vertices, UVs, normals and materials.
*/

#pragma once

#include <GLM/glm.hpp>
#include <vector>

#include "../../3D/Materials/Material.h"

namespace Aela {
	class SubModel {
		public:
			SubModel() {

			}

			// These are the getters and setters of the class.
			std::vector<glm::vec3>* getVertices();
			void setVertices(std::vector<glm::vec3>* vertices);
			std::vector<glm::vec2>* getUVs();
			void setUVs(std::vector<glm::vec2>* UVs);
			std::vector<glm::vec3>* getNormals();
			void setNormals(std::vector<glm::vec3>* normals);
			std::vector<unsigned short>* getIndices();
			void setIndices(std::vector<unsigned short>* indices);
			size_t getVertexSize();
			size_t getUVSize();
			size_t getNormalSize();
			size_t getIndexSize();
			Material* getMaterial();
			void setMaterial(Material* material);

		private:
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec2> UVs;
			std::vector<glm::vec3> normals;
			std::vector<unsigned short> indices;
			Material* material;
	};
}