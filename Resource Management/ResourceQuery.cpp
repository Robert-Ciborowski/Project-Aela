#include "ResourceQuery.h"

// Resource class implementation

Aela::ResourceQuery::ResourceQuery(std::string src, bool crucial, Aela::ResourceLoader* loader) : src(src), crucial(crucial), loader(loader) {
}

Aela::ResourceQuery::~ResourceQuery() {
}

std::string Aela::ResourceQuery::getSrc() {
	return src;
}

bool Aela::ResourceQuery::isCrucial() {
	return crucial;
}

Aela::ResourceLoader* Aela::ResourceQuery::getLoader() {
	return loader;
}