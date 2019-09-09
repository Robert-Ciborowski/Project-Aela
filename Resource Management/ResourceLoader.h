#pragma once

#include "Resource.h"
#include "ResourceMap.h"
#include <unordered_map>

namespace Aela {
	class ResourceLoader {
		public:
			virtual bool load(ResourceMap& resources, std::string src) = 0;

			std::string getErrorMessage();
		protected:
			std::string errorMessage;

			bool isValid(std::ifstream& in);
			std::istream& getline(std::ifstream& in, std::string& line);
	};
}