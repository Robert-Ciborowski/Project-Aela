#include <utility>
#include "ResourceManager.h"
#include "../Error Handler/ErrorHandling.h"
#include <ios>

using namespace Aela;

ResourceManager::ResourceManager(unsigned int resourceCount) {
	resources.reserve(resourceCount);
}

ResourceManager::~ResourceManager() {};

void ResourceManager::bindLoader(ResourceLoader* loader) {
	this->boundLoader = loader;
}

void ResourceManager::bindGroup(std::string group) {
	// ensure a group exists to add to
	auto iter = groups.find(group);
	if (iter == groups.end()) {
		groups.emplace(group, ResourceGroup(group));
		this->boundGroup = &(groups.find(group)->second);
	} else {
		this->boundGroup = &(iter->second);
	}
}

ResourceManager::Status ResourceManager::loadGroup(std::string name) {
	// ensure a group exists to load
	auto iter = groups.find(name);
	if (iter == groups.end()) {
		return Status::FAILED;
	}

	// get the group to load
	auto group = &(iter->second);

	if (group->getQueries()->empty()) {
		return Status::EMPTY;
	}

	// load all resources in the group
	Status returnStatus = Status::OK;

	for (ResourceQuery query : *(group->getQueries())) {
		// make sure we are using the correct resource loader
		ResourceLoader* resourceLoader = query.getLoader();
		if (boundLoader != resourceLoader) {
			bindLoader(resourceLoader);
		}

		// load the resource
		Status status = load(query);

		// interpret the result
		if (status == Status::ABORT) {
			return status;
		}
		if (status == Status::FAILED) {
			returnStatus = status;
		}
	}
	return returnStatus;
}

ResourceManager::Status ResourceManager::unloadGroup(std::string name) {
	// ensure a group exists to unload
	auto iter = groups.find(name);
	if (iter == groups.end()) {
		return Status::FAILED;
	}

	// get the group to unload
	auto group = &(iter->second);

	// unload all resources in the group
	for (ResourceQuery query : *(group->getQueries())) {
		// unload the resource
		unload(query.getSrc());
	}
	return Status::OK;
}

void ResourceManager::setResourceRoot(std::string resourceRoot) {
	resources.setResourceRoot(std::move(resourceRoot));
}

std::string Aela::ResourceManager::getResourceRoot() {
	return resources.getResourceRoot();
}

void ResourceManager::addToGroup(std::string src, bool crucial) {
	ResourceQuery query(src, crucial, boundLoader);
	addToGroup(query);
}

void ResourceManager::addToGroup(ResourceQuery& query) {
	boundGroup->getQueries()->push_back(query);
}

bool ResourceManager::groupExists(std::string name) {
	auto iter = groups.find(name);
	if (iter == groups.end()) {
		return false;
	}
	return true;
}

ResourceManager::Status ResourceManager::load(std::string src, bool crucial, ResourceLoader& loader) {
	ResourceQuery query(src, crucial, &loader);
	return load(query);
}

ResourceManager::Status ResourceManager::load(ResourceQuery& query) {
	bool crucial = query.isCrucial();
	std::string src = query.getSrc();
	if (resources.isResourceRootEnabled()) {
		src = resources.getResourceRoot() + src;
	}

	// TODO: Bug where if a resource loader has duplicate resources, the line below crashes.
	bool success = boundLoader->load(resources, src);

	if (!success) {
		// cannot load the resource
		if (crucial) {
			crucialInvalidResourceKey = src;
			return Status::ABORT;
		} else {
			invalidResourceKeys.push_back(src);
			return Status::FAILED;
		}
	}

	return Status::OK;
}

void ResourceManager::unload(std::string src) {
	Resource* res;

	if (resources.get(std::move(src), res) && res != nullptr) {
		delete res;
	}
}

void Aela::ResourceManager::useResourceRoot(bool resourceRootEnabled) {
	resources.useResourceRoot(resourceRootEnabled);
}

bool ResourceManager::isResourceRootEnabled() {
	return resources.isResourceRootEnabled();
}

std::string Aela::ResourceManager::getNewCrucialInvalidResourceKey() {
	return crucialInvalidResourceKey;
}

std::vector<std::string>& Aela::ResourceManager::getNewInvalidResourceKeys() {
	return invalidResourceKeys;
}
