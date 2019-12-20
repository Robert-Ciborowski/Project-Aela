/*
* Class: XML Loader
* Author: Robert Ciborowski
* Date: 29/07/2019
* Description: A class used to load text files in the XML format.
*/

#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include <unordered_map>

namespace Aela {
	using namespace rapidxml;

	class XMLLoader {
	public:
		XMLLoader(const char* TOP_NODE_NAME);
		virtual ~XMLLoader();

	protected:
		struct Node {
			std::string name;
			std::string value;
			std::unordered_map<std::string, std::string> attributes;
			Node* parent;
		};

		const char* TOP_NODE_NAME;

		// These should be used to start the loading.
		bool loadFromFile(std::string& src);
		bool loadFromFile(std::wstring& src);
		bool loadFromCompressedFile(std::string& src);
		bool loadFromCompressedFile(std::wstring& src);
		void loadFromText(char* text);

		// This function is called upon each XML Node/Tag.
		virtual void processXMLNodeData(Node& node);

	private:
		// Note that if a node contains children node, processNode
		// gets called on the children before the parent node.
		void processNode(xml_node<>* node, Node* parent);
	};
}