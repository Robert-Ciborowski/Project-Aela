/*
* Class: AbstractLayoutManager
* Author: Robert Ciborowski
* Date: 02/04/2017
* Description: Sizes and positions components within a parent container.
*/

#pragma once
#include "Container.h"
#include "LayoutManager.h"

namespace Aela {
	class AbstractLayoutManager : public LayoutManager {
		public:
			AbstractLayoutManager(Container* parent);
			~AbstractLayoutManager();

			virtual void update(std::vector<std::shared_ptr<Component>>& components);
		protected:
			Container* parent;
	};
}
