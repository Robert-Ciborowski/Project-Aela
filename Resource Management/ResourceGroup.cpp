#include "ResourceGroup.h"

using namespace Aela;

ResourceGroup::ResourceGroup(std::string name) : name(name) {
}

ResourceGroup::~ResourceGroup() {
}

std::vector<ResourceQuery>* ResourceGroup::getQueries() {
	return &queries;
}