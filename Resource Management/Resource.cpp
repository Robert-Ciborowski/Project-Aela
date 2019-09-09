#include "Resource.h"

using namespace Aela;

Resource::Resource(std::string src) : src(src) {
}

Resource::~Resource() {
}

std::string Resource::getSrc() {
	return src;
}