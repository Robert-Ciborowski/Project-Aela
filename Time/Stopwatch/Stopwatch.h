/*
* Class: Stopwatch
* Author: Robert Ciborowski
* Date: 06/07/2018
* Description: A class used to measure the total and average time
               of one or more sets of operations.
			   
			   Think of this as a stopwatch that can record the lap
			   times of multiple runners at once, as well as their
			   average lap times. But instead of runners, this class
			   can record the duration of any portion of code.
*/

#pragma once
#include "../Clock.h"
#include <unordered_map>
#include <string>

namespace Aela {
	class Stopwatch {
		public:
			Stopwatch();

			void startRecording(std::string tag);
			void stopRecording(std::string tag);
			long long getTotalTimeInNanos(std::string tag);
			long long getTotalTimeInMicros(std::string tag);
			long long getTotalTimeInMillis(std::string tag);
			long long getTotalTimeInSeconds(std::string tag);
			long long getAverageTimeInNanos(std::string tag);
			long long getAverageTimeInMicros(std::string tag);
			long long getAverageTimeInMillis(std::string tag);
			long long getAverageTimeInSeconds(std::string tag);
			void outputTimesIntoConsole();
			void reset();

		private:
			struct Timeable {
				long long totalTime = 0;
				long long numberOfRecordings = 0;
				long long timeOfLastRecording = 0;
				long long startTimeOfCurrentRecording = 0;
				bool recording = false;
			};

			std::unordered_map<std::string, Timeable> timeables;

			Clock time;
	};
}