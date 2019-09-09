/*
* Class: Material Loader
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for loading material files.
*/

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#include "GLMaterialLoader.h"
#include "Material.h"
#include "../../Error Handler/ErrorHandling.h"

#include "../../Utilities/strut.h"

using namespace Aela;

GLMaterialLoader::GLMaterialLoader() = default;

GLMaterialLoader::~GLMaterialLoader() = default;
bool Aela::GLMaterialLoader::load(ResourceMap& resources, std::string src) {
	Material* material = nullptr;
	std::string line;

	std::ifstream in(src, std::ios::binary);

	if (!isValid(in)) {
		return false;
	}

	// This begins reading the file.
	if (in.is_open()) {
		while (getline(in, line)) {
			if (line.find("newmtl ") != std::string::npos) {
				material = new Material(src);
				std::string name = line.substr(MATERIAL_NAME_START_POSITION, line.size() - MATERIAL_NAME_START_POSITION);
				resources.put(src + "/" + name, material);
			} else if (line.find("map_Kd ") != std::string::npos || line.find("map_Ka ") != std::string::npos) {
				if (material == nullptr) {
					// Error! The file did not specify a material name before setting the texture!
					return false;
				}

				// If this part of the code is reached, the line may look like "map_Kd tire_1.dds".
				std::string fileName = line.substr(MATERIAL_PATH_START_POSITION, line.size() - MATERIAL_PATH_START_POSITION);
				trim(fileName);

				// This retrieves the folder path.
				std::string path = "/";
				for (size_t i = src.length() - 1; i >= 0; i--) {
					if (src.at(i) == '/') {
						path = src.substr(0, i + 1);
						break;
					}
				}
				
				std::string textureName = path + fileName;
				GLTexture* texture;
				if (GLTextureLoader::load(resources, textureName)) {
					resources.get(textureName, texture);
					material->setTexture(texture);
				}
			} else {
				// The line in the file is treated as a comment if this line is reached.
			}
		}
	} else {
		AelaErrorHandling::windowError("Aela Material Loader", "The program tried to load a non-existent mtl file.");
		in.close();
		return false;
	}
	in.close();
	return true;
}