/*
* Class: OBJ Loader
* Author: Robert Ciborowski
* Date: 04/07/2017
* Description: A class for loading OBJ files into models.
*/

#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>

#include <glm/glm.hpp>

#include "OBJLoader.h"
#include "../../Resource Management/ResourcePaths.h"
#include "../../Error Handler/ErrorHandling.h"
#include "../../Utilities/Safe Functions.h"

using namespace Aela;

OBJLoader::OBJLoader() = default;

OBJLoader::~OBJLoader() = default;

bool Aela::OBJLoader::load(ResourceMap& resources, std::string src) {
	// try to open the file
	std::ifstream in(src, std::ios::binary);
	if (!isValid(in)) {
		AelaErrorHandling::consoleWindowError("Aela OBJ Loader", "The file " + src + " could not be found.");
		return false;
	}

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	Model* res = new Model(src);

	// I'm removing this feature for now, since it's rather important to be consistent throughout Aela in resource conventions.
	// In addition, this currently does not save any memory since an extra property was added to every resource.
	// Please talk to me about re-adding this feature if you need it.

	/*// This checks to see if the resource can use the "~" shortcut for its path to signify that the resource is using the
	// commonly excepted path for its resource type. Doing this saves memory.
	if (src.substr(0, defaultTexturePath.size()) == defaultTexturePath) {
		result->setSource("~" + src.substr(defaultTexturePath.size(), src.size()));
	} else {
		result->setSource(src);
	}*/
	std::string materialName;

	// This actually reads the file.
	std::string line;

	while (getline(in, line)) {
		// This reads the first word of the line.
		if (line.find("o ") != std::string::npos) {
			if (!res->getSubModels()->empty()) {
				setupSubModel(&res->getSubModels()->at(res->getSubModels()->size() - 1), &vertexIndices, &uvIndices, &normalIndices, &tempVertices, &tempUVs, &tempNormals);
			}
			SubModel subModel;
			res->getSubModels()->push_back(subModel);
		} else if (line.find("usemtl ") != std::string::npos) {
			materialName = line.substr(7, line.size() - 7);

			// use ~ for default path
			if (materialName.at(0) == '~') {
				materialName = materialName.substr(1);
				materialName = DEFAULT_MATERIAL_PATH + materialName;
			}
			Material* material;
			bool success = resources.get<Material>(materialName, material);
			if (!success) {
				AelaErrorHandling::consoleWindowError("Aela OBJ Loader", "The material " + materialName + " requested by " + src + " was not found." +
					" Are you sure that you loaded the material before loading the object?");
				return false;
			}
			res->getSubModels()->at(res->getSubModels()->size() - 1).setMaterial(material);
		} else if (line.find("v ") != std::string::npos) {
			glm::vec3 vertex;
			line.erase(0, 2);
			sscanf_safe(line.c_str(), "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		} else if (line.find("vt ") != std::string::npos) {
			glm::vec2 uv;
			line.erase(0, 3);
			sscanf_safe(line.c_str(), "%f %f\n", &uv.x, &uv.y);
			// This wil invert the V coordinate since this uses a DDS texture, which are inverted. If you use BMPS then don't do this!
			uv.y = -uv.y;
			tempUVs.push_back(uv);
		} else if (line.find("vn ") != std::string::npos) {
			line.erase(0, 3);
			glm::vec3 normal;
			sscanf_safe(line.c_str(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
		} else if (line.find("f ") != std::string::npos) {
			line.erase(0, 2);
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int numberOfSlashes = 0;
			for (char i : line) {
				if (i == '/') {
					numberOfSlashes++;
				}
			}
			if (numberOfSlashes == 6) {
				int combiner = sscanf_safe(line.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (combiner == 9) {
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				} else {
					// Error, somehow.
				}
			} else if (numberOfSlashes == 3) {
				AelaErrorHandling::windowError("Aela OBJ Model Loader", src + (std::string) ": The requested baseModel's normal (vn) information is missing.\n"
					+ "Try exporting the baseModel with different information.");
			} else {
				AelaErrorHandling::windowError("Aela OBJ Model Loader", src + (std::string) ": The formatting of the face ('f') section of the OBJ file\nis "
					+ "not the same that the loader uses.\nMake sure to specify vertex, UV\nand normal data in your OBJ file!");
				in.close();
				delete res;
				return false;
			}
				
		} else {
			// The line in the file is a comment if this line is reached.
		}
	}

	in.close();

	setupSubModel(&res->getSubModels()->at(res->getSubModels()->size() - 1), &vertexIndices, &uvIndices, &normalIndices, &tempVertices, &tempUVs, 
		&tempNormals);
	resources.put(src, res);
	return true;
}

void Aela::OBJLoader::setupSubModel(SubModel* subModel, std::vector<unsigned int>* vertexIndices, std::vector<unsigned int>* uvIndices,
	std::vector<unsigned int>* normalIndices, std::vector<glm::vec3>* tempVertices, std::vector<glm::vec2>* tempUVs,
	std::vector<glm::vec3>* tempNormals) {
	std::vector<glm::vec3> sortedVertices;
	std::vector<glm::vec2> sortedUVs;
	std::vector<glm::vec3> sortedNormals;
	for (unsigned int i = 0; i < vertexIndices->size(); i++) {
		// This will get the indexes.
		unsigned int vertexIndex = vertexIndices->at(i);
		unsigned int normalIndex = normalIndices->at(i);
		unsigned int uvIndex = uvIndices->at(i);

		// This will get the attributes.
		glm::vec3 vertex = tempVertices->at(vertexIndex - 1);
		glm::vec3 normal = tempNormals->at(normalIndex - 1);
		glm::vec2 uv = tempUVs->at(uvIndex - 1);

		// This will put the attributes in the buffers.
		sortedVertices.push_back(vertex);
		sortedUVs.push_back(uv);
		sortedNormals.push_back(normal);
	}

	indexVBO(&sortedVertices, &sortedUVs, &sortedNormals, subModel->getIndices(), subModel->getVertices(), subModel->getUVs(), subModel->getNormals());

	vertexIndices->clear();
	uvIndices->clear();
	normalIndices->clear();
}

bool Aela::OBJLoader::getSimilarVertex(OBJLoader::VertexPacket* data, std::map<VertexPacket, unsigned short>* vertexDataMap, unsigned short* result) {
	auto it = vertexDataMap->find(*data);
	if (it == vertexDataMap->end()) {
		return false;
	} else {
		*result = it->second;
		return true;
	}
}

void Aela::OBJLoader::indexVBO(std::vector<glm::vec3>* inputVertices, std::vector<glm::vec2>* inputUVs, std::vector<glm::vec3>* inputNormals,
	std::vector<unsigned short>* outputIndices, std::vector<glm::vec3>* outputVertices, std::vector<glm::vec2>* outputUVs,
	std::vector<glm::vec3>* outputNormals) {
	std::map<VertexPacket, unsigned short> vertexDataMap;

	for (unsigned int i = 0; i < inputVertices->size(); i++) {
		VertexPacket packed = {inputVertices->at(i), inputUVs->at(i), inputNormals->at(i)};

		// This checks to see if the vertex already exists.
		unsigned short index;
		bool exists = getSimilarVertex(&packed, &vertexDataMap, &index);

		if (exists) {
			outputIndices->push_back(index);
		} else {
			outputVertices->push_back(inputVertices->at(i));
			outputUVs->push_back(inputUVs->at(i));
			outputNormals->push_back(inputNormals->at(i));
			auto newindex = static_cast<unsigned short>(outputVertices->size() - 1);
			outputIndices->push_back(newindex);
			vertexDataMap[packed] = newindex;
		}
	}
}
