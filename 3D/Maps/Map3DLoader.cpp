#include <utility>

/*
* Class: Map3D Loader
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to load a map file into a Map3D.
*/

#include "Map3DLoader.h"
#include "../../Utilities/strut.h"
#include <glm/gtc/constants.hpp>
#include "../../Resource Management/ResourcePaths.h"

using namespace Aela;
using namespace rapidxml;

Aela::Map3DLoader::Map3DLoader(std::string resourceRoot) : XMLLoader("Map3D") {
	this->resourceRoot = std::move(resourceRoot);
}

Aela::Map3DLoader::~Map3DLoader() = default;

bool Aela::Map3DLoader::load(ResourceMap& resources, std::string src) {
	// This tries to open the file.
	/*std::ifstream in(src, std::ios::binary);
	if (!isValid(in)) {
		AelaErrorHandling::consoleWindowError("Aela Map Loader", "The map " + src + " was not found.");
		return false;
	}*/

	this->resources = &resources;
	map = new Map3D(src);

	XMLLoader::loadFromFile(src);

	// After reading the XML, add the map as a resource.
	resources.put(src, map);
	return true;
}

void Aela::Map3DLoader::bindRenderer(GLRenderer* renderer) {
	this->renderer = renderer;
}

void Aela::Map3DLoader::processXMLNodeData(Aela::Map3DLoader::Node& node) {
	size_t entityID = 0;

	if (node.name == "Model") {
		entityID = map->getModels()->size();
		ModelEntity modelEntity;
		auto iter = node.attributes.find("src");

		if (iter != node.attributes.end()) {
			Resource* res;
			std::string source = iter->second;

			if (source.at(0) == '~') {
				source = DEFAULT_MODEL_PATH + source.substr(1, source.size() - 1);
			}

			bool success = resources->get(source, res);

			if (success) {
				map->getModel(entityID)->setModel(( Model*) res);
			} else {
				return;
			}
		}

		iter = node.attributes.find("position");

		if (iter != node.attributes.end()) {
			std::string value = std::string(iter->second);
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			map->getModel(entityID)->setPosition(vec3);
		}

		iter = node.attributes.find("rotation");

		if (iter != node.attributes.end()) {
			std::string value = std::string(iter->second);
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			map->getModel(entityID)->setRotation(vec3);
		}

		iter = node.attributes.find("scaling");

		if (iter != node.attributes.end()) {
			std::string value = std::string(iter->second);
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			map->getModel(entityID)->setScaling(vec3);
		}

		map->addModel(entityID, &modelEntity);
	} else if (node.name == "Light") {
		entityID = map->getLights()->size();
		LightEntity lightEntity;
		auto iter = node.attributes.find("power");

		if (iter != node.attributes.end()) {
			std::string value = iter->second;
			float power = std::stof(value);
			lightEntity.setPower(power);
		}

		iter = node.attributes.find("colour");

		if (iter != node.attributes.end()) {
			std::string value = iter->second;
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			// TODO fix
			lightEntity.setColour(&(ColourRGB(&vec3)));
		}

		if (iter != node.attributes.end()) {
			std::string value = std::string(iter->second);
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			lightEntity.setPosition(vec3);
		}

		iter = node.attributes.find("rotation");

		if (iter != node.attributes.end()) {
			std::string value = std::string(iter->second);
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			lightEntity.setRotation(vec3);
		}

		iter = node.attributes.find("scaling");

		if (iter != node.attributes.end()) {
			std::string value = std::string(iter->second);
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			lightEntity.setScaling(vec3);
		}

		map->addLight(entityID, &lightEntity);
	} else if (node.name == "AmbientLight") {
		auto iter = node.attributes.find("power");

		if (iter != node.attributes.end()) {
			std::string value = iter->second;
			float power = std::stof(value);
			map->setAmbientLighting(power);
		}
	} else if (node.name == "Skybox") {
		entityID = map->getSkyboxes()->size();
		SkyboxEntity skyboxEntity;
		auto iter = node.attributes.find("src");

		if (iter != node.attributes.end()) {
			Resource* res;
			std::string source = iter->second;

			if (source.at(0) == '~') {
				source = DEFAULT_SKYBOX_PATH + source.substr(1, source.size() - 1);
			}

			bool success = resources->get(source, res);

			if (success) {
				skyboxEntity.setSkybox((Skybox*) res);
			} else {
				return;
			}
		}

		map->addSkybox(entityID, &skyboxEntity);
	} else if (node.name == "Billboard") {
		entityID = map->getBillboards()->size();
		BillboardEntity billboardEntity;
		auto iter = node.attributes.find("src");

		if (iter != node.attributes.end()) {
			Resource* res;
			std::string source = iter->second;

			if (source.at(0) == '~') {
				source = DEFAULT_TEXTURE_PATH + source.substr(1, source.size() - 1);
			}

			bool success = resources->get(source, res);

			if (success) {
				billboardEntity.setTexture(( Texture*) res);
			} else {
				return;
			}
		}

		iter = node.attributes.find("position");

		if (iter != node.attributes.end()) {
			std::string value = std::string(iter->second);
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			billboardEntity.setPosition(vec3);
		}

		iter = node.attributes.find("rotation");

		if (iter != node.attributes.end()) {
			std::string value = std::string(iter->second);
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			billboardEntity.setRotation(vec3);
		}

		iter = node.attributes.find("scaling");

		if (iter != node.attributes.end()) {
			std::string value = std::string(iter->second);
			glm::vec3 vec3;
			setVec3UsingString(&value, &vec3);
			billboardEntity.setScaling(vec3);
		}

		iter = node.attributes.find("useRotation");

		if (iter != node.attributes.end()) {
			std::string value = iter->second;
			if (value == "false") {
				billboardEntity.useSpecifiedRotation(false);
			}
		}

		map->addBillboard(entityID, &billboardEntity);
	}
}