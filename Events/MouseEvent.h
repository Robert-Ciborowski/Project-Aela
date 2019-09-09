#pragma once

#include "Event.h"

namespace Aela {
	class MouseEvent : public Event {
	private:
		int button;
		int modifier;
		int clicks;
		int x, y;
		int rendererX, rendererY;
		int wheel;

	public:
		MouseEvent();
		MouseEvent(int _type, int _button, int _modifier, int _clicks, int _x, int _y, int _rendererX, int _rendererY, int _wheel);
		virtual ~MouseEvent();

		int getButton();
		int getModifier();
		int getClicks();
		int getMouseX();
		int getMouseY();
		int getMouseXInRendererOutput();
		int getMouseYInRendererOutput();
		int getWheel();
	};
}