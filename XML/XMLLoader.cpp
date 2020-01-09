#include "XMLLoader.h"
#include "../Error Handler/ErrorHandling.h"
#include "../Compression/Compressor.h"
#include "../Utilities/enumut.h"
#include <sstream>
#include <iostream>

using namespace rapidxml;

Aela::XMLLoader::XMLLoader(const char* TOP_NODE_NAME) {
	this->TOP_NODE_NAME = TOP_NODE_NAME;
}

Aela::XMLLoader::~XMLLoader() {}

bool Aela::XMLLoader::loadFromFile(std::string& src) {
	try {
		xml_document<> doc;
		file<> xmlFile(src.c_str());
		doc.parse<0>(xmlFile.data());

		// This reads the XML by starting at the top node.
		xml_node<>* node = doc.first_node(TOP_NODE_NAME);
		processNode(node, nullptr);
	} catch (std::runtime_error e) {
		AelaErrorHandling::consoleWindowError("XML Loader", "Could not open uncompressed xml file!");
		return false;
	}

	return true;
}

bool Aela::XMLLoader::loadFromFile(std::wstring& src) {
	try {
		xml_document<> doc;
		std::ifstream stream;
		stream.open(src, std::ifstream::in);
		file<> xmlFile(stream);
		doc.parse<0>(xmlFile.data());

		// This reads the XML by starting at the top node.
		xml_node<>* node = doc.first_node(TOP_NODE_NAME);
		processNode(node, nullptr);
	} catch (std::runtime_error e) {
		AelaErrorHandling::consoleWindowError("XML Loader", "Could not open uncompressed xml file!");
		std::wcout << src << "\n";
		return false;
	}

	return true;
}

bool Aela::XMLLoader::loadFromCompressedFile(std::string& src) {
	CompressionError ret;
	std::ifstream input;
	std::stringstream stringstream;

	Compressor compressor;
	input.open(src, std::ifstream::in | std::ios::binary);

	if (!input.is_open()) {
		AelaErrorHandling::consoleWindowError("XML Loader", "Could not open compressed xml file!");
		input.close();
		return false;
	}

	ret = compressor.decompressStream(&input, &stringstream);

	if (ret != CompressionError::OK) {
		AelaErrorHandling::consoleWindowError("World Loader", "An error occured while decompressing xml file: " + std::to_string(enumToInteger(ret)));
		input.close();
		return false;
	}

	input.close();
	std::string str = stringstream.str();
	size_t count = str.size();
	char* data = (char*) malloc(count + 1);
	memcpy_s(data, count + 1, str.c_str(), count + 1);
	loadFromText(data);
	free(data);
	return true;
}

bool Aela::XMLLoader::loadFromCompressedFile(std::wstring& src) {
	CompressionError ret;
	std::ifstream input;
	std::stringstream stringstream;

	Compressor compressor;
	input.open(src, std::ifstream::in | std::ios::binary);

	if (!input.is_open()) {
		AelaErrorHandling::consoleWindowError("XML Loader", "Could not open compressed xml file!");
		input.close();
		return false;
	}

	ret = compressor.decompressStream(&input, &stringstream);

	if (ret != CompressionError::OK) {
		AelaErrorHandling::consoleWindowError("World Loader", "An error occured while decompressing xml file: " + std::to_string(enumToInteger(ret)));
		input.close();
		return false;
	}

	input.close();
	std::string str = stringstream.str();
	size_t count = str.size();
	char* data = (char*) malloc(count + 1);
	memcpy_s(data, count + 1, str.c_str(), count + 1);
	loadFromText(data);
	free(data);
	return true;
}

void Aela::XMLLoader::loadFromText(char* text) {
	xml_document<> doc;
	doc.parse<0>(text);

	// This reads the XML by starting at the top node.
	xml_node<>* node = doc.first_node(TOP_NODE_NAME);

	processNode(node, nullptr);
}

void Aela::XMLLoader::processXMLNodeData(Node& node) {
	// This is for the subclass to fill in.
}

void Aela::XMLLoader::processNode(xml_node<>* node, Node* parent) {
	if (node == nullptr) {
		AelaErrorHandling::consoleWindowWarning("A node was nullptr when loading XML?!");
		return;
	}

	Node nodeStruct;
	nodeStruct.name = node->name();

	if (nodeStruct.name == "") {
		return;
	}

	nodeStruct.value = node->value();
	nodeStruct.parent = parent;

	for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
		nodeStruct.attributes[attr->name()] = attr->value();
	}

	for (xml_node<>* node2 = node->first_node(); node2; node2 = node2->next_sibling()) {
		processNode(node2, &nodeStruct);
	}

	processXMLNodeData(nodeStruct);
}
