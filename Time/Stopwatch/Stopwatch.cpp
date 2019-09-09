#include "Stopwatch.h"
#include <iostream>

Aela::Stopwatch::Stopwatch() {}

void Aela::Stopwatch::startRecording(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		Timeable timeable;
		timeables[tag] = timeable;
		pos = timeables.find(tag);
	}

	pos->second.recording = true;
	time.updateTime();
	pos->second.startTimeOfCurrentRecording = time.getCurrentTimeInNanos();
}

void Aela::Stopwatch::stopRecording(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		return;
	}

	if (pos->second.recording) {
		pos->second.recording = false;
		time.updateTime();
		long long recordingTime = time.getCurrentTimeInNanos() - pos->second.startTimeOfCurrentRecording;
		pos->second.timeOfLastRecording = recordingTime;
		pos->second.totalTime += recordingTime;
		pos->second.numberOfRecordings++;
	}
}

long long Aela::Stopwatch::getTotalTimeInNanos(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		return 0;
	}
	
	return pos->second.totalTime;
}

long long Aela::Stopwatch::getTotalTimeInMicros(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		return 0;
	}

	return pos->second.totalTime / 1000;
}

long long Aela::Stopwatch::getTotalTimeInMillis(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		return 0;
	}

	return pos->second.totalTime / 1000000;
}

long long Aela::Stopwatch::getTotalTimeInSeconds(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		return 0;
	}

	return pos->second.totalTime / 1000000000;
}

long long Aela::Stopwatch::getAverageTimeInNanos(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		return 0;
	}

	if (pos->second.numberOfRecordings == 0) {
		return 0;
	}

	return pos->second.totalTime / pos->second.numberOfRecordings;
}

long long Aela::Stopwatch::getAverageTimeInMicros(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		return 0;
	}

	if (pos->second.numberOfRecordings == 0) {
		return 0;
	}

	return pos->second.totalTime / pos->second.numberOfRecordings / 1000;
}

long long Aela::Stopwatch::getAverageTimeInMillis(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		return 0;
	}

	if (pos->second.numberOfRecordings == 0) {
		return 0;
	}

	return pos->second.totalTime / pos->second.numberOfRecordings / 1000000;
}

long long Aela::Stopwatch::getAverageTimeInSeconds(std::string tag) {
	auto pos = timeables.find(tag);
	if (pos == timeables.end()) {
		return 0;
	}

	if (pos->second.numberOfRecordings == 0) {
		return 0;
	}

	return pos->second.totalTime / pos->second.numberOfRecordings / 1000000000;
}

void Aela::Stopwatch::outputTimesIntoConsole() {
	std::cout << "----------------------------------------------------\n";
	std::cout << "- Stopwatch times:                                 -\n";
	std::cout << "----------------------------------------------------\n";

	long long sumOfAverages = 0;
	for (auto& pair : timeables) {
		Timeable& timeable = pair.second;
		if (timeable.numberOfRecordings == 0) {
			std::cout << "- " << pair.first << " - Number of Recordings: 0\n";
		} else {
			double average = (double) timeable.totalTime / timeable.numberOfRecordings;
			std::cout << "- " << pair.first << " - Average Clock: " << average << " ns, Total Clock: "
				<< timeable.totalTime << " ns, # of Recordings: " << timeable.numberOfRecordings << "\n";
			sumOfAverages += (long long) average;
		}
	}

	std::cout << "- \n- \n- % Breakdown:\n";

	for (auto& pair : timeables) {
		Timeable& timeable = pair.second;
		std::cout << "- " << pair.first << ": " << (double) timeable.totalTime / timeable.numberOfRecordings / sumOfAverages * 100 << "%\n";
	}

	std::cout << "\n----------------------------------------------------\n\n";
}

void Aela::Stopwatch::reset() {
	timeables.clear();
}