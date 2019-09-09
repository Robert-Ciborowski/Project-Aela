/*
* Class: Container
* Author: Robert Ciborowski
* Date: 20/03/2017
* Description: Represents a container in our menu system. It lives inside another container,
*              and can contain various containers, widgets and other assorted GUI elements.
*/

#pragma once
#include "Component.h"
#include "LayoutManager.h"
#include <vector>
#include <memory>

namespace Aela {
	class Container : public Component {
		public:
			Container();
			virtual ~Container();

			void add(std::shared_ptr<Component> component);
			bool remove(std::shared_ptr<Component> component);
			void clear();

			virtual void onMousePressed(MouseEvent* event);
			virtual void onMouseReleased(MouseEvent* event);
			virtual void onMouseMoved(MouseEvent* event);

			virtual void markDirty();

		protected:
			Container(int x, int y);
			std::vector<std::shared_ptr<Component>> children;

		private:
			LayoutManager* layout;

			virtual void updateComponent();
			virtual void renderComponent(GLRenderer& renderer);
	};
}
