/*
* Class: Resource
* Author: Robert Ciborowski
* Date: 01/01/2017
* Description: Represents a resource used in the application.
*/

#pragma once

#include <string>

namespace Aela {
	class Resource {
		public:
			Resource(std::string src);
			virtual ~Resource();

			// you shouldn't copy resources
			// But then we can't use them with boost::python
			// Resource(const Resource&) = delete;
			// void operator=(const Resource&) = delete;

			std::string getSrc();
		private:
			std::string src;
	};
}