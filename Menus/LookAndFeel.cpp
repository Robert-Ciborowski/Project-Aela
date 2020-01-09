#include "LookAndFeel.h"

void Aela::LookAndFeel::putAttribute(std::string key, std::string value) {
	attributes.emplace(key, value);
}
std::string Aela::LookAndFeel::getAttribute(std::string key) {
	auto iter = attributes.find(key);
	if (iter == attributes.end()) {
		return "";
	}

	return iter->second;
}
