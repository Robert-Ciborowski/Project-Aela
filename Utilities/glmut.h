/*
* Name: GLM Utilities
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A file used for utilities regarding the glm library.
*/

#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

// The following structs are for when you want to create a HashMap with glm::vecs as the keys.
struct Vec2HashMapFunctions {
	size_t operator()(const glm::vec2& k) const {
		return std::hash<float>()(k.x) ^ std::hash<float>()(k.y);
	}

	bool operator()(const glm::vec2& a, const glm::vec2& b) const {
		return a.x == b.x && a.y == b.y;
	}
};

struct IVec2HashMapFunctions {
	size_t operator()(const glm::ivec2& k) const {
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
	}

	bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
		return a.x == b.x && a.y == b.y;
	}
};

struct Vec3HashMapFunctions {
	size_t operator()(const glm::vec3& k) const {
		return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
	}

	bool operator()(const glm::vec3& a, const glm::vec3& b) const {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

struct IVec3HashMapFunctions {
	size_t operator()(const glm::ivec3& k) const {
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
	}

	bool operator()(const glm::ivec3& a, const glm::ivec3& b) const {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

struct Vec4HashMapFunctions {
	size_t operator()(const glm::vec4& k) const {
		return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z) ^ std::hash<float>()(k.a);
	}

	bool operator()(const glm::vec4& a, const glm::vec4& b) const {
		return a.x == b.x && a.y == b.y && a.z == b.z && a.a == b.a;
	}
};

// This function is used to force a value within a specific range (not clamping).
// For example, if using the range 0 and 2a, 11a / 2 is forced to 3a / 2.
static void forceWithinRange(glm::vec3* vec3, float minimum, float maximum) {
	if (vec3->x < minimum) {
		vec3->x += (maximum - minimum) * ceil(abs((minimum - vec3->x) / (maximum - minimum)));
	}

	if (vec3->x > maximum) {
		vec3->x -= (maximum - minimum) * ceil(abs((maximum - vec3->x) / (maximum - minimum)));
	}

	if (vec3->y < minimum) {
		vec3->y += (maximum - minimum) * ceil(abs((minimum - vec3->y) / (maximum - minimum)));
	}

	if (vec3->y > maximum) {
		vec3->y -= (maximum - minimum) * ceil(abs((maximum - vec3->y) / (maximum - minimum)));
	}

	if (vec3->z < minimum) {
		vec3->z += (maximum - minimum) * ceil(abs((minimum - vec3->z) / (maximum - minimum)));
	}

	if (vec3->z > maximum) {
		vec3->z -= (maximum - minimum) * ceil(abs((maximum - vec3->z) / (maximum - minimum)));
	}
}

static std::string vecToString(glm::vec2* vec) {
	return "(" + std::to_string(vec->x) + ", " + std::to_string(vec->y) + ")";
}

static std::string vecToString(glm::vec3* vec) {
	return "(" + std::to_string(vec->x) + ", " + std::to_string(vec->y) + ", " + std::to_string(vec->z) + ")";
}

static std::string vecToString(glm::vec4* vec) {
	return "(" + std::to_string(vec->x) + ", " + std::to_string(vec->y) + ", " + std::to_string(vec->z) + ", " + std::to_string(vec->w) + ")";
}