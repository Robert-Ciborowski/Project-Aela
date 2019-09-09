/*
* Class: Pool
* Author: Julian Dominguez-Schatz
* Date: 03/01/2018
* Description: Represents an audio clip resource used in the application.
*/

#pragma once

#include "../../Resource Management/Resource.h"
#include <unordered_set>
#include <queue>
#include <al.h>

namespace Aela {
	template <class T>
	class Pool {
		public:
			Pool(PoolSource* source) : source(source) {
			}

			Pool(PoolSource* source, int initialSize) : source(source) {

				for (T* object : objects) {
					T* object = source->createObject();
					objects.push(object);
				}
			}

			virtual ~Pool() noexcept {
				for (T* object : objects) {
					source->destroyObject(object);
				}
			}

			T* obtain() {
				if (freeObjects.empty()) {
					return freeObjects.pop();
				}

				T* object = source->createObject();
				objects.push(object);
				return object;
			}

			bool free(T* object) {
				if (!objects.contains(object)) {
					return false;
				}

				source->resetObject(object);
				freeObjects.push(object);
				return true;
			}
		private:
			std::unordered_set<T*> objects;
			std::queue<T*> freeObjects;

			PoolSource* source;
	};
}