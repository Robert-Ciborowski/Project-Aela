/*
* Class: Timer Functions
* Author: Robert Ciborowski
* Date: 30/06/2019
* Description: Timer functions for Python scripts. Some of these
               functions exist because of how std::function gets transferred
			   between Python and C++.
*/

#pragma once
#include <string>
#include "../Time/Timer/Timer.h"
#include "../Utilities/pyut.h"

namespace PythonFunctions {
	using namespace Aela;

	std::string scheduleEventInNanos(Timer& self, long long waitTime, object object) {
		return self.scheduleEventInNanos(waitTime, convertToFunc(object));
	}

	std::string scheduleEventInMicros(Timer& self, long long waitTime, object object) {
		return self.scheduleEventInMicros(waitTime, convertToFunc(object));
	}

	std::string scheduleEventInMillis(Timer& self, long long waitTime, object object) {
		return self.scheduleEventInMillis(waitTime, convertToFunc(object));
	}

	std::string scheduleEventInSeconds(Timer& self, long long waitTime, object object) {
		return self.scheduleEventInSeconds(waitTime, convertToFunc(object));
	}

	void scheduleEventInNanosUsingTag(Timer& self, const std::string tag, long long waitTime, object object) {
		self.scheduleEventInNanosUsingTag(tag, waitTime, convertToFunc(object));
	}

	void scheduleEventInMicrosUsingTag(Timer& self, const std::string tag, long long waitTime, object object) {
		self.scheduleEventInMicrosUsingTag(tag, waitTime, convertToFunc(object));
	}

	void scheduleEventInMillisUsingTag(Timer& self, const std::string tag, long long waitTime, object object) {
		self.scheduleEventInMillisUsingTag(tag, waitTime, convertToFunc(object));
	}

	void scheduleEventInSecondsUsingTag(Timer& self, const std::string tag, long long waitTime, object object) {
		self.scheduleEventInSecondsUsingTag(tag, waitTime, convertToFunc(object));
	}
}