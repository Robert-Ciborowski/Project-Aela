/*
* Class: LayoutManager
* Author: Robert Ciborowski
* Date: 27/03/2017
* Description: Sizes and positions components within a parent container.
*/

#pragma once
#include "Component.h"
#include <vector>
#include <memory>

namespace Aela {
	class LayoutManager {
		public:
			virtual void update(std::vector<std::shared_ptr<Component>>& components) = 0;
	};
}
