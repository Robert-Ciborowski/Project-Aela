/*
* Class: SubModel
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for an object, which represents part of a model. It uses vertices, UVs, normals and materials.
*/

#include "SubModel.h"

using namespace Aela;

std::vector<glm::vec3>* Aela::SubModel::getVertices() {
	return &vertices;
}

void Aela::SubModel::setVertices(std::vector<glm::vec3>* vertices) {
	this->vertices = *vertices;
}

std::vector<glm::vec2>* Aela::SubModel::getUVs() {
	return &UVs;
}

void Aela::SubModel::setUVs(std::vector<glm::vec2>* UVs) {
	this->UVs = *UVs;
}

std::vector<glm::vec3>* Aela::SubModel::getNormals() {
	return &normals;
}

void Aela::SubModel::setNormals(std::vector<glm::vec3>* normals) {
	this->normals = *normals;
}

std::vector<unsigned short>* Aela::SubModel::getIndices() {
	return &indices;
}

void Aela::SubModel::setIndices(std::vector<unsigned short>* indices) {
	this->indices = *indices;
}

size_t Aela::SubModel::getVertexSize() {
	return vertices.size();
}

size_t Aela::SubModel::getUVSize() {
	return UVs.size();
}

size_t Aela::SubModel::getNormalSize() {
	return normals.size();
}

size_t Aela::SubModel::getIndexSize() {
	return indices.size();
}

Material* Aela::SubModel::getMaterial() {
	return material;
}

void Aela::SubModel::setMaterial(Material* material) {
	this->material = material;
}
