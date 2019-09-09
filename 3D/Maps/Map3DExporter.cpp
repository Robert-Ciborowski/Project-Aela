/*
* Class: Map3D Exporter
* Author: Robert Ciborowski
* Date: 22/08/2017
* Description: A class used to export a Map3D to a map file.
*/

#include "Map3DExporter.h"
#include "../../Resource Management/ResourcePaths.h"
#include "../../Utilities/strut.h"

using namespace Aela;

void Aela::Map3DExporter::abbreviate(std::string& src, std::string defaultPath) {
	if (startsWith(src, defaultPath)) {
		src = "~" + src.substr(defaultPath.size(), src.size());
	}
}

bool Aela::Map3DExporter::exportMap(std::string path, Map3D* map, bool mapIsReadable) {
	std::ofstream file;
	file.open(path);
	
	bool success;
	if (file.is_open()) {
		success = true;

		file << "<Map3D>\n";

		for (auto& pair : *map->getSkyboxes()) {
			SkyboxEntity* entity = &pair.second;
			if (entity->getSkybox() != nullptr && !entity->getSkybox()->getSrc().empty()) {
				file << "<Skybox src=\"";
				std::string skyboxPath = entity->getSkybox()->getSrc();
				abbreviate(skyboxPath, DEFAULT_SKYBOX_PATH);

				file << skyboxPath << "\"/>";
				if (mapIsReadable) {
					file << "\n";
				}
			}
		}

		for (auto& pair : *map->getModels()) {
			ModelEntity* entity = &pair.second;
			if (entity->getModel() != nullptr && !entity->getModel()->getSrc().empty()) {
				file << "<Model src=\"";
				std::string modelPath = entity->getModel()->getSrc();
				abbreviate(modelPath, DEFAULT_MODEL_PATH);

				file << modelPath << "\"";
				file << getTransformableString(entity) << "/>";
				if (mapIsReadable) {
					file << "\n";
				}
			}
		}

		for (auto& pair : *map->getLights()) {
			LightEntity* entity = &pair.second;
			file << "<Light";
			file << getLightString(entity);
			file << getTransformableString(entity) << "/>";
			if (mapIsReadable) {
				file << "\n";
			}
		}

		for (auto& pair : *map->getBillboards()) {
			BillboardEntity* entity = &pair.second;
			if (entity->getTexture() != nullptr && !entity->getTexture()->getSrc().empty()) {
				file << "<Billboard src=\"";
				std::string texturePath = entity->getTexture()->getSrc();
				abbreviate(texturePath, DEFAULT_TEXTURE_PATH);

				file << texturePath << "\"";
				file << getBillboardString(entity);
				file << getTransformableString(entity) << "/>";
				if (mapIsReadable) {
					file << "\n";
				}
			}
		}

		file << "</Map3D>\n";
	} else {
		success = false;
	}
	file.close();
	return success;
}

std::string Aela::Map3DExporter::getTransformableString(Transformable3D* transformable) {
	std::string string;
	if (*transformable->getPosition() != glm::vec3(0, 0, 0)) {
		glm::vec3* vec3 = transformable->getPosition();
		string += " position=\"" + toStringWithATrailingZero(vec3->x) + "," + toStringWithATrailingZero(vec3->y) + ","
			+ toStringWithATrailingZero(vec3->z) + "\"";
	}
	if (*transformable->getRotation() != glm::vec3(0, 0, 0)) {
		glm::vec3* vec3 = transformable->getRotation();
		string += " rotation=\"" + toStringWithATrailingZero(vec3->x) + "," + toStringWithATrailingZero(vec3->y) + ","
			+ toStringWithATrailingZero(vec3->z) + "\"";
	}
	if (*transformable->getScaling() != glm::vec3(0, 0, 0)) {
		glm::vec3* vec3 = transformable->getScaling();
		string += " scaling=\"" + toStringWithATrailingZero(vec3->x) + "," + toStringWithATrailingZero(vec3->y) + ","
			+ toStringWithATrailingZero(vec3->z) + "\"";
	}
	return string;
}

std::string Aela::Map3DExporter::getLightString(LightEntity* light) {
	std::string string;
	glm::vec3 colour = light->getColour()->getVec3();
	if (colour != glm::vec3(1, 1, 1)) {
		string += " colour=\"" + toStringWithATrailingZero(colour.x) + "," + toStringWithATrailingZero(colour.y) + ","
			+ toStringWithATrailingZero(colour.z) + "\"";
	}
	if (light->getPower() != 1) {
		string += " power\"" + toStringWithATrailingZero(light->getPower()) + "\"";
	}
	return string;
}

std::string Aela::Map3DExporter::getBillboardString(BillboardEntity* billboard) {
	std::string string;
	if (!billboard->usingSpecifiedRotation()) {
		string += " useRotation=\"true\"";
	}
	return string;
}
