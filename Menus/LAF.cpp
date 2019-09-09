#include "LAF.h"

void Aela::LAF::putAttribute(std::string key, std::string value) {
	attributes.emplace(key, value);
}
std::string Aela::LAF::getAttribute(std::string key) {
	auto iter = attributes.find(key);
	if (iter == attributes.end()) {
		return "";
	}

	return iter->second;
}
