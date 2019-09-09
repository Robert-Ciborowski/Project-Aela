#pragma once

namespace Aela {
	class Event {
		public:
			Event(int _type);
			virtual ~Event();

			void consume();

			int getType();
			bool isConsumed();
		private:
			int type;
			bool consumed = false;
	};
}