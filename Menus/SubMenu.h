/*
* Class: SubMenu
* Author: Robert Ciborowski
* Date: 24/12/2017
* Description: A menu that you put inside of another menu. This is necessary so that menu rendering
*              and other things work properly.
*/

#pragma once
#include "Menu.h"

namespace Aela {
	class SubMenu : public Menu {
		public:
			SubMenu() {}
			virtual ~SubMenu() {}

			virtual void render(GLRenderer& renderer);
	};
}
