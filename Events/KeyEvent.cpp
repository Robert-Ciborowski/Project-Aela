#include "KeyEvent.h"

using namespace Aela;

KeyEvent::KeyEvent(int _type, int _keycode, int _modifier) : Event(_type) {
	keycode = _keycode;
	modifier = _modifier;
}

KeyEvent::~KeyEvent() {

}

int KeyEvent::getKeycode() {
	return keycode;
}

int KeyEvent::getModifier() {
	return modifier;
}