/*
* Class: Resource Functions
* Author: Robert Ciborowski
* Date: 02/07/2019
* Description: ResourceManager binding to use for Python scripts.
               See the bottom for the actual binding part itself.
*/

#pragma once
#include <boost/python.hpp>
#include <string>
#include "../Resource Management/ResourceManager.h"
#include "../2D/Texture/GLSpriteSheetLoader.h"
#include "../Audio/AudioClip.h"
#include "../2D/Fonts/Font.h"
#include "../2D/Fonts/FontLoader.h"
#include "../3D/Materials/GLMaterialLoader.h"
#include "../3D/Models/OBJLoader.h"
#include "../2D/Texture/GLTextureLoader.h"
#include "../3D/Skybox/GLSkyboxLoader.h"
#include "../3D/Maps/Map3DLoader.h"
#include "../Audio/WAVEClipLoader.h"
#include "../2D/Fonts/FontLoader.h"
#include "../Audio/OGGClipLoader.h"
#include "../Audio/OGGStreamLoader.h"

namespace PythonFunctions {
	using namespace Aela;
	using namespace boost::python;

	AudioClip* obtainAudioClip(ResourceManager& self, const std::string src) {
		AudioClip* clip;
		if (!self.obtain(src, clip)) {
			return nullptr;
		}

		return clip;
	}

	Font* obtainFont(ResourceManager& self, const std::string src) {
		Font* font;
		if (!self.obtain(src, font)) {
			return nullptr;
		}

		return font;
	}

	Material* obtainMaterial(ResourceManager& self, const std::string src) {
		Material* material;
		if (!self.obtain(src, material)) {
			return nullptr;
		}

		return material;
	}

	Model* obtainModel(ResourceManager& self, const std::string src) {
		Model* model;
		if (!self.obtain(src, model)) {
			return nullptr;
		}

		return model;
	}

	GLTexture* obtainGLTexture(ResourceManager& self, const std::string src) {
		GLTexture* texture;
		if (!self.obtain(src, texture)) {
			return nullptr;
		}

		return texture;
	}

	bool loadMaterials(ResourceManager& self, const std::string& defaultMaterialPath, list& materialNames) {
		GLMaterialLoader materialLoader;
		self.bindLoader(&materialLoader);
		std::string groupName = std::string("materials_") + (std::string) extract<std::string>(materialNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(materialNames); i++) {
			std::string path = extract<std::string>(materialNames[i]);
			self.addToGroup(defaultMaterialPath + path + ".mtl", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadOBJModels(ResourceManager& self, const std::string& defaultModelPath, list& modelNames) {
		OBJLoader objLoader;
		self.bindLoader(&objLoader);
		std::string groupName = std::string("obj_models_") + (std::string) extract<std::string>(modelNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(modelNames); i++) {
			std::string path = extract<std::string>(modelNames[i]);
			self.addToGroup(defaultModelPath + path + ".obj", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadPNGTextures(ResourceManager& self, const std::string& defaultTexturePath, list& textureNames) {
		GLTextureLoader textureLoader;
		self.bindLoader(&textureLoader);
		std::string groupName = std::string("png_textures_") + (std::string) extract<std::string>(textureNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(textureNames); i++) {
			std::string path = extract<std::string>(textureNames[i]);
			self.addToGroup(defaultTexturePath + path + ".png", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadPNGSpriteSheets(ResourceManager& self, const std::string& defaultSpriteSheetPath, list& spriteSheetNames,
		int spriteWidth, int spriteHeight) {
		GLSpriteSheetLoader spriteSheetLoader;
		spriteSheetLoader.setSpriteWidth(spriteWidth);
		spriteSheetLoader.setSpriteHeight(spriteHeight);
		self.bindLoader(&spriteSheetLoader);
		std::string groupName = std::string("png_sprite_sheets_") + (std::string) extract<std::string>(spriteSheetNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(spriteSheetNames); i++) {
			std::string path = extract<std::string>(spriteSheetNames[i]);
			self.addToGroup(defaultSpriteSheetPath + path + ".png", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadParticles(ResourceManager& self, const std::string& defaultParticlePath, list& particleNames) {
		// We may want to create a seperate particle loader later.
		GLTextureLoader textureLoader;
		self.bindLoader(&textureLoader);
		std::string groupName = std::string("particles_") + (std::string) extract<std::string>(particleNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(particleNames); i++) {
			std::string path = extract<std::string>(particleNames[i]);
			self.addToGroup(defaultParticlePath + path + ".dds", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadDDSTextures(ResourceManager& self, const std::string& defaultTexturePath, list& textureNames) {
		GLTextureLoader textureLoader;
		self.bindLoader(&textureLoader);
		std::string groupName = std::string("dds_textures_") + (std::string) extract<std::string>(textureNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(textureNames); i++) {
			std::string path = extract<std::string>(textureNames[i]);
			self.addToGroup(defaultTexturePath + path + ".dds", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadSkyboxes(ResourceManager& self, const std::string& defaultSkyboxPath, list& skyboxNames) {
		GLSkyboxLoader skyboxLoader;
		self.bindLoader(&skyboxLoader);
		std::string groupName = std::string("skyboxes_") + (std::string) extract<std::string>(skyboxNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(skyboxNames); i++) {
			std::string path = extract<std::string>(skyboxNames[i]);
			self.addToGroup(defaultSkyboxPath + path, false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadMaps(ResourceManager& self, const std::string& defaultMapPath, list& mapNames) {
		Map3DLoader mapLoader(self.getResourceRoot());
		self.bindLoader(&mapLoader);
		std::string groupName = std::string("maps_") + (std::string) extract<std::string>(mapNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(mapNames); i++) {
			std::string path = extract<std::string>(mapNames[i]);
			self.addToGroup(defaultMapPath + path + ".txt", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadWaveSoundEffectsUnlessLoaded(ResourceManager& self, const std::string& defaultAudioPath, list& audioNames) {
		WAVEClipLoader waveClipLoader;
		self.bindLoader(&waveClipLoader);
		std::string groupName = std::string("wave_sound_effects_") + (std::string) extract<std::string>(audioNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(audioNames); i++) {
			AudioClip* clip;
			std::string path = extract<std::string>(audioNames[i]);
			if (self.obtain<AudioClip>(defaultAudioPath + path + ".wav", clip)) {
				continue;
			}
			self.addToGroup(defaultAudioPath + path + ".wav", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadWaveMusicUnlessLoaded(ResourceManager& self, const std::string& defaultMusicPath, list& musicNames) {
		// Since we don't have a better audio loader right now, use clips:
		WAVEClipLoader waveClipLoader;
		self.bindLoader(&waveClipLoader);
		std::string groupName = std::string("wave_music_") + (std::string) extract<std::string>(musicNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(musicNames); i++) {
			AudioClip* clip;
			std::string path = extract<std::string>(musicNames[i]);
			if (self.obtain<AudioClip>(defaultMusicPath + path + ".wav", clip)) {
				continue;
			}
			self.addToGroup(defaultMusicPath + path + ".wav", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadOggSoundEffectsUnlessLoaded(ResourceManager& self, const std::string& defaultAudioPath, list& audioNames) {
		OGGClipLoader oggClipLoader;
		self.bindLoader(&oggClipLoader);
		std::string groupName = std::string("ogg_sound_effects_") + (std::string) extract<std::string>(audioNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(audioNames); i++) {
			AudioClip* clip;
			std::string path = extract<std::string>(audioNames[i]);
			if (self.obtain<AudioClip>(defaultAudioPath + path + ".ogg", clip)) {
				continue;
			}
			self.addToGroup(defaultAudioPath + path + ".ogg", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadOggMusicUnlessLoaded(ResourceManager& self, const std::string& defaultMusicPath, list& musicNames) {
		// Since we don't have a better audio loader right now, use clips:
		OGGStreamLoader oggStreamLoader;
		self.bindLoader(&oggStreamLoader);
		std::string groupName = std::string("ogg_music_") + (std::string) extract<std::string>(musicNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(musicNames); i++) {
			AudioStream* stream;
			std::string path = extract<std::string>(musicNames[i]);
			if (self.obtain<AudioStream>(defaultMusicPath + path + ".ogg", stream)) {
				continue;
			}
			self.addToGroup(defaultMusicPath + path + ".ogg", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}

	bool loadFonts(ResourceManager& self, const std::string& defaultFontPath, list& fontNames) {
		FontLoader fontLoader;
		self.bindLoader(&fontLoader);
		std::string groupName = std::string("ttf_fonts_") + (std::string) extract<std::string>(fontNames[0]);
		self.bindGroup(groupName);

		for (int i = 0; i < len(fontNames); i++) {
			std::string path = extract<std::string>(fontNames[i]);
			self.addToGroup(defaultFontPath + path + ".ttf", false);
		}

		if (self.loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << self.getNewInvalidResourceKeys()[0] << "\n";
			return false;
		}

		return true;
	}
}