#include "ResourceLoader.h"
#include "../Utilities/strut.h"
#include <fstream>

std::string Aela::ResourceLoader::getErrorMessage() {
	return errorMessage;
}

bool Aela::ResourceLoader::isValid(std::ifstream& in) {
	return (in.is_open() && in.good() && !in.fail() && !in.bad());
}

std::istream& Aela::ResourceLoader::getline(std::ifstream& in, std::string& line) {
	auto& ret = std::getline(in, line);
	trim(line);
	return ret;
}
