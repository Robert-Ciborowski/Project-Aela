#pragma once

#include "Event.h"

namespace Aela {
	class KeyEvent : public Event {
		private:
			int keycode;
			int modifier;
		public:
			KeyEvent(int _type, int _keycode, int _modifier);
			virtual ~KeyEvent();

			int getKeycode();
			int getModifier();
	};
}