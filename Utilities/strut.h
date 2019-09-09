/*
* Name: String Utilities
* Author: Robert Ciborowski
* Date: August 2017
* Description: A file used for performing operations on strings.
*/

#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <sstream>

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

static inline std::string toStringWithDecimal(float value, int places) {
	std::string s = std::to_string(value);
	s = s.substr(0, glm::clamp((int) s.find('.') + places + 1, 0, (int) s.size()));
	return s;
}

static std::string toStringWithATrailingZero(float value) {
	std::string s = std::to_string(value);
	bool trailing = false;
	int trailingPos = 0;
	for (unsigned int pos = 0; pos < s.length(); pos++) {
		if (s.at(pos) == '0' && !trailing) {
			trailing = true;
			trailingPos = pos;
		} else if (s.at(pos) != '0') {
			trailing = false;
		}
	}
	if (trailing) {
		s = s.substr(0, trailingPos + 1);
	}
	return s;
}

static inline bool startsWith(std::string& haystack, std::string& needle) {
	return needle.length() <= haystack.length()
		&& equal(needle.begin(), needle.end(), haystack.begin());
}

static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> values;
    std::string value;
    std::istringstream stream(s);
    while (std::getline(stream, value, delimiter)) {
		values.push_back(value);
    }
	if (stream.rdbuf()->in_avail() != 0) {
		std::getline(stream, value);
		values.push_back(value);
	}
	return values;
}

static std::vector<std::string> split(std::istringstream& s, char delimiter) {
    std::vector<std::string> values;
    std::string value;
    while (std::getline(s, value, delimiter)) {
		values.push_back(value);
    }
	if (s.rdbuf()->in_avail() != 0) {
		std::getline(s, value);
		values.push_back(value);
	}
   return values;
}

static std::vector<std::string> split(std::stringstream& s, char delimiter) {
    std::vector<std::string> values;
    std::string value;
	while (std::getline(s, value, delimiter)) {
		values.push_back(value);
	}
	if (s.rdbuf()->in_avail() != 0) {
		std::getline(s, value);
		values.push_back(value);
	}
    return values;
}
	

static void setVec2UsingString(std::string* value, glm::vec2* vec2) {
	std::vector<std::string> values;

	for (unsigned int l = 0; l < value->size(); l++) {
		if (value->at(l) == ',') {
			values.push_back(value->substr(0, l));
			value->erase(0, l + 1);
			l = -1;
		} else if (l == value->size() - 1) {
			values.push_back(value->substr(0, l + 1));
			break;
		}
	}

	if (values.size() > 1) {
		float floatValues[2];

		for (unsigned int i = 0; i < 2; i++) {
			if (values.at(i) == "PI") {
				floatValues[i] = glm::pi<float>();
			} else if (values.at(i) == "2PI") {
				// Note: Setting a rotational value of a Transformabe3D to "2PI" should just set it to zero.
				floatValues[i] = glm::pi<float>() * 2;
			} else {
				floatValues[i] = std::stof(values.at(i));
			}
		}

		*vec2 = glm::vec2(floatValues[0], floatValues[1]);
	}
}

static void setVec2UsingString(std::string* value, glm::ivec2* ivec2) {
	std::vector<std::string> values;

	for (unsigned int l = 0; l < value->size(); l++) {
		if (value->at(l) == ',') {
			values.push_back(value->substr(0, l));
			value->erase(0, l + 1);
			l = -1;
		} else if (l == value->size() - 1) {
			values.push_back(value->substr(0, l + 1));
			break;
		}
	}

	if (values.size() > 1) {
		int intValues[2];

		for (unsigned int i = 0; i < 2; i++) {
			intValues[i] = std::stoi(values.at(i));
		}

		*ivec2 = glm::ivec2(intValues[0], intValues[1]);
	}
}
static void setVec3UsingString(std::string* value, glm::vec3* vec3) {
	std::vector<std::string> values;

	for (unsigned int l = 0; l < value->size(); l++) {
		if (value->at(l) == ',') {
			values.push_back(value->substr(0, l));
			value->erase(0, l + 1);
			l = -1;
		} else if (l == value->size() - 1) {
			values.push_back(value->substr(0, l + 1));
			break;
		}
	}

	if (values.size() > 2) {
		float floatValues[3];

		for (unsigned int i = 0; i < 3; i++) {
			if (values.at(i) == "PI") {
				floatValues[i] = glm::pi<float>();
			} else if (values.at(i) == "2PI") {
				// Note: Setting a rotational value of a Transformabe3D to "2PI" should just set it to zero.
				floatValues[i] = glm::pi<float>() * 2;
			} else {
				floatValues[i] = std::stof(values.at(i));
			}
		}

		*vec3 = glm::vec3(floatValues[0], floatValues[1], floatValues[2]);
	}
}

static void setVec3UsingString(std::string* value, glm::ivec3* ivec3) {
	std::vector<std::string> values;

	for (unsigned int l = 0; l < value->size(); l++) {
		if (value->at(l) == ',') {
			values.push_back(value->substr(0, l));
			value->erase(0, l + 1);
			l = -1;
		} else if (l == value->size() - 1) {
			values.push_back(value->substr(0, l + 1));
			break;
		}
	}

	if (values.size() > 2) {
		int intValues[3];

		for (unsigned int i = 0; i < 3; i++) {
			intValues[i] = std::stoi(values.at(i));
		}

		*ivec3 = glm::ivec3(intValues[0], intValues[1], intValues[2]);
	}
}
// This returns a string that express the individual bits of an unsigned char.
static std::string stringOfBits(unsigned char input) {
	std::string s = "";
	for (int i = 0; i < 8; i++) {
		if (input % 2 == 0) {
			s = "0" + s;
		} else {
			s = "1" + s;
		}
		input = input >> 1;
	}
	return s;
}
