/*
* Class: ResourceMap
* Author: Robert Ciborowski
* Date: 03/08/2017
* Description: Represents a map of strings to resources used in the application.
*/

#pragma once

#include <unordered_map>
#include <utility>

#include "Resource.h"

namespace Aela {
	class ResourceMap {
		public:
			ResourceMap();
			virtual ~ResourceMap();

			void reserve(unsigned int count);

			bool contains(std::string key);

			template <class T> bool get(std::string key, T*& result) {
				result = (T*) get_impl(std::move(key));
				return (result != nullptr);
			}

			void put(std::string key, Resource* value);

			void setResourceRoot(std::string resourceRoot);
			std::string getResourceRoot();

			bool isResourceRootEnabled();
			void useResourceRoot(bool resourceRootEnabled);

		private:
			std::unordered_map<std::string, Resource*> resources;

			Resource* get_impl(std::string key);

			std::string resourceRoot = "../../";
			bool resourceRootEnabled = true;
	};
}