/*
* Class: SubMenu
* Author: Robert Ciborowski
* Date: 24/12/2017
* Description: A menu that you put inside of another menu. This is necessary so that menu rendering
*              and other things work properly.
*/

#include "SubMenu.h"

using namespace Aela;

void SubMenu::render(GLRenderer& renderer) {
	if (visible && initialized && isDirty()) {
		Container::render(renderer);
	}
}
