/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: idk
* Description: A class used to hold constants.
*/

#pragma once

namespace Aela {
	class EventConstants {
		public:
			static constexpr int KEY_PRESSED{0};
			static constexpr int KEY_RELEASED{1};

			static constexpr int MOUSE_PRESSED{2};
			static constexpr int MOUSE_RELEASED{3};
			// "MOUSE_MOVED" would be better, but Visual Studio interprets that as a macro or something,
			// meaning that projects which use Aela won't compile!
			static constexpr int MOUSE_MOTIONED{4};
			static constexpr int MOUSE_ENTERED{5};
			static constexpr int MOUSE_EXITED{6};

			static constexpr int MOUSE_ENTERED_COMPONENT{7};
			static constexpr int MOUSE_EXITED_COMPONENT{8};
			static constexpr int MOUSE_WHEEL{9};

			static constexpr int WINDOW_RESIZE{10};
	};
}
