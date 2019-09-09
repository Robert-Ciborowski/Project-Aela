/*
* Class: Map3D 3D
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to store the 3D entities of a map/world.
*/

#include "Map3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace Aela;

SkyboxEntity* Map3D::getSkybox(long long id) {
	return &skyboxes[id];
}

LightEntity* Map3D::getLight(long long id) {
	return &lights[id];
}

ModelEntity* Map3D::getModel(long long id) {
	return &models[id];
}

BillboardEntity* Map3D::getBillboard(long long id) {
	return &billboards[id];
}

void Map3D::removeLight(long long id) {
	lights.erase(id);
}

void Map3D::removeModel(long long id) {
	auto iter = std::find(resourceGroupedModelsWithoutTransparency[models[id].getModel()].begin(), resourceGroupedModelsWithoutTransparency[models[id].getModel()].end(), id);
	if (iter != resourceGroupedModelsWithoutTransparency[models[id].getModel()].end()) {
		resourceGroupedModelsWithoutTransparency[models[id].getModel()].erase(iter);
	}
	for (auto& layer : resourceGroupedModelsWithTransparency) {
		auto unorderedMap = layer.second->at(models[id].getModel());
		iter = std::find(unorderedMap.begin(), unorderedMap.end(), id);
		if (iter != unorderedMap.end()) {
			unorderedMap.erase(iter);
		}
	}
	for (auto& iter1 : resourceGroupedModelsWithoutTransparency) {
		for (auto& value : iter1.second) {
			if (value > id) {
				value--;
			}
		}
	}
	for (auto& iter1 : resourceGroupedModelsWithTransparency) {
		for (auto& iter2 : *iter1.second) {
			for (auto& value : iter2.second) {
				if (value > id) {
					value--;
				}
			}
		}
	}
	models.erase(id);
}

void Map3D::removeBillboard(long long id) {
	billboards.erase(id);
}

void Map3D::removeSkybox(long long id) {
	skyboxes.erase(id);
}

void Map3D::removeAllLights() {
	lights.clear();
}

void Map3D::removeAllBillboards() {
	billboards.clear();
}

void Map3D::removeAllSkyboxes() {
	skyboxes.clear();
}

void Map3D::removeAllModels() {
	models.clear();
	resourceGroupedModelsWithoutTransparency.clear();

	for (auto& iter : resourceGroupedModelsWithTransparency) {
		delete iter.second;
	}
	resourceGroupedModelsWithTransparency.clear();
}

std::unordered_map<long long, LightEntity>* Map3D::getLights() {
	return &lights;
}

std::unordered_map<long long, ModelEntity>* Map3D::getModels() {
	return &models;
}

std::unordered_map<long long, BillboardEntity>* Map3D::getBillboards() {
	return &billboards;
}

std::unordered_map<long long, SkyboxEntity>* Map3D::getSkyboxes() {
	return &skyboxes;
}

ModelResourceMap* Map3D::getResourceGroupedModelsWithoutTransparency() {
	return &resourceGroupedModelsWithoutTransparency;
}

std::map<size_t, ModelResourceMap*>* Map3D::getResourceGroupedModelsWithTransparency() {
	return &resourceGroupedModelsWithTransparency;
}

void Aela::Map3D::addModelTransparencyLayer(size_t layer) {
	if (resourceGroupedModelsWithTransparency.find(layer) == resourceGroupedModelsWithTransparency.end()) {
		resourceGroupedModelsWithTransparency[layer] = new ModelResourceMap();
	}
}

void Map3D::addLight(long long id, LightEntity* light) {
	lights[id] = *light;
}

void Map3D::addModelWithTransparency(long long id, ModelEntity* model, size_t layer) {
	addModelWithoutGeneratingData(id, model);
	generateAdditionalModelDataWithTransparency(id, layer);
}

void Map3D::addModel(long long id, ModelEntity* model) {
	addModelWithoutGeneratingData(id, model);
	generateAdditionalModelDataWithoutTransparency(id);
}

void Map3D::addModelWithoutGeneratingData(long long id, ModelEntity* model) {
	if (model != nullptr) {
		models[id] = *model;
	}
}

void Map3D::generateAdditionalModelDataWithoutTransparency(long long whichModel) {
	resourceGroupedModelsWithoutTransparency[models[whichModel].getModel()].push_back(whichModel);
}

void Map3D::generateAdditionalModelDataWithTransparency(long long whichModel, size_t layer) {
	auto pos = resourceGroupedModelsWithTransparency.find(layer);
	if (pos != resourceGroupedModelsWithTransparency.end()) {
		(*pos->second)[models[whichModel].getModel()].push_back(whichModel);
	}
}

void Map3D::removeAdditionalModelData(bool modelsAreTransparent) {
	if (modelsAreTransparent) {
		resourceGroupedModelsWithTransparency.clear();
	} else {
		resourceGroupedModelsWithoutTransparency.clear();
	}
}

void Map3D::removeEverything() {
	removeAllModels();
	removeAdditionalModelData(true);
	removeAdditionalModelData(false);
	removeAllBillboards();
	removeAllLights();
	removeAllSkyboxes();
}

void Aela::Map3D::setAmbientLighting(float ambientLighting) {
	this->ambientLighting = ambientLighting;
}

float Aela::Map3D::getAmbientLighting() {
	return ambientLighting;
}


void Map3D::addSkybox(long long id, SkyboxEntity* skybox) {
	skyboxes[id] = *skybox;
}

void Map3D::addBillboard(long long id, BillboardEntity* billboard) {
	billboards[id] = *billboard;
}