#include "SourceTree.h"
#include "SourceTreeEndNode.cpp"
#include <iostream>
#include <string>

using namespace Where1::typify::Parsing;
using namespace std;

SourceTree::SourceTree(string contents) : contents(contents)
{
	ParseChildren();
}

void SourceTree::ParseInnards(int* pi, bool is_named_structure, string structure_type = "") {
	this->structure_type = structure_type;

	int i = *pi;
	if (is_named_structure) {
		string targetString = structure_type;

		i = contents.find(targetString);
		int identifierLeft = i + targetString.length();

		i = contents.find("{", i);
		int identifierRight = i;

		string structure_identifier = contents.substr(identifierLeft, identifierRight - identifierLeft);
		while (structure_identifier.find(" ") != std::string::npos) {
			int idI = structure_identifier.find(" ");
			structure_identifier.erase(idI, 1);
		}

		while (structure_identifier.find("\t") != std::string::npos) {
			int idI = structure_identifier.find("\t");
			structure_identifier.erase(idI, 1);

		}

		this->structure_identifier = structure_identifier;
	}




	int typeStart = i;

	i = contents.find("{", i) + 1;

	int depth = 1;
	int start = i;
	for (i; i < contents.length(); i++) {
		if (contents[i] == '{') {
			depth++;
		}
		else if (contents[i] == '}') {
			depth--;
			if (depth == 0) {
				break;
			}
		}
	}

	string innerTypeContents = contents.substr(start, i - start);

	//cout << innerTypeContents;

	original_contents = contents;
	contents = contents.substr(0, typeStart) + contents.substr(i + 1);


	int i2 = 0;
	while (innerTypeContents.find("\n", i2) != std::string::npos) {
		int start = innerTypeContents.find("\n", i2) + 1;
		int end = innerTypeContents.find("\n", start);
		if (end == std::string::npos) {
			break;
		}
		i2 = end;

		string curr = innerTypeContents.substr(start, end - start);

		bool is_complex_structure = true;
		for (int j = 0; j < curr.length(); j++) {
			if (curr[j] != ' ' && curr[j] != '\t' && curr[j] != '{') {
				is_complex_structure = false;
				break;
			}
		}

		if (is_complex_structure) {
			int endBrace = innerTypeContents.find("}", i2) + 1;
			i2 = endBrace; // Don't try and parse this twice, esp not in the wrong context

			SourceTree* child = new SourceTree(innerTypeContents.substr(start, endBrace - start));
			children.push_back(child);
		}
		else {
			SourceTreeEndNode* child = new SourceTreeEndNode(curr);
			end_nodes.push_back(child);
		}
	}

	*pi = i;
}

void SourceTree::ParseChildren() {
	int i = 0;

	string structure_types[] = { "interface", "class", "struct" };
	for (int j = 0; j < structure_types->length(); j++) {
		while (contents.find(structure_types[j], i) != std::string::npos) {
			ParseInnards(&i, true, structure_types[j]);
		}
	}

	while (contents.find("{", i) != std::string::npos) {
		ParseInnards(&i, false);
	}
}
