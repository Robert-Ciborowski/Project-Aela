#include "AbstractLayoutManager.h"

using namespace Aela;

AbstractLayoutManager::AbstractLayoutManager(Container* parent) : parent(parent) {
}

AbstractLayoutManager::~AbstractLayoutManager() {
}

void AbstractLayoutManager::update(std::vector<std::shared_ptr<Component>>& components) {
	// TODO: include code for updating child position and size
}