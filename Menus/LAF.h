/*
 * Class: LAF
 * Author: Julian Dominguez-Schatz
 * Date: 16/08/2017
 * Description: A class which represents a look-and-feel for UI widgets. This provides
 * a set of default parameters to UI components so the user doesn't have to initialize them every time.
 */

#pragma once

#include <string>
#include <unordered_map>

namespace Aela {
	class LAF {
		public:
			void putAttribute(std::string key, std::string value);
			std::string getAttribute(std::string key);

		private:
			std::unordered_map<std::string, std::string> attributes;
	};
}