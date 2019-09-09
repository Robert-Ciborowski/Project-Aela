#include "ResourceMap.h"

using namespace Aela;

ResourceMap::ResourceMap() = default;

ResourceMap::~ResourceMap() = default;

void ResourceMap::reserve(unsigned int count) {
	resources.reserve(count);
}

bool ResourceMap::contains(std::string src) {
	auto iter = resources.find(src);
	return iter != resources.end();
}

Resource* ResourceMap::get_impl(std::string key) {
	if (resourceRootEnabled) {
		key = resourceRoot + key;
	}

	auto iter = resources.find(key);
	if (iter == resources.end()) {
		return nullptr;
	}
	return iter->second;
}

void ResourceMap::put(std::string src, Resource* value) {
	resources[src] = value;
}

bool ResourceMap::isResourceRootEnabled() {
	return resourceRootEnabled;
}

void ResourceMap::useResourceRoot(bool resourceRootEnabled) {
	this->resourceRootEnabled = resourceRootEnabled;
}

void ResourceMap::setResourceRoot(std::string resourceRoot) {
	this->resourceRoot = std::move(resourceRoot);
}

std::string ResourceMap::getResourceRoot() {
	// This is actually necessary. If a program wants to load a file without using the ResourceManager
	// (such as if the file isn't meant to be an actual resource), they can have access to the root.
	return resourceRoot;
}
