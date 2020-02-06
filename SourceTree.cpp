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

void SourceTree::ParseChildren() {
	int i = 0;
	while (contents.find("interface", i) != std::string::npos) {
		string targetString = "interface";

		i = contents.find(targetString);
		int identifierLeft = i + targetString.length();

		i = contents.find("{", i);
		int identifierRight = i;

		string interfaceIdentifier = contents.substr(identifierLeft, identifierRight - identifierLeft);
		while (interfaceIdentifier.find(" ") != std::string::npos) {
			int idI = interfaceIdentifier.find(" ");
			interfaceIdentifier.erase(idI, 1);
		}

		while (interfaceIdentifier.find("\t") != std::string::npos) {
			int idI = interfaceIdentifier.find("\t");
			interfaceIdentifier.erase(idI, 1);

		}

		this->interfaceIdentifier = interfaceIdentifier;



		int typeStart = i;

		i = contents.find("{", i) + 1;

		int depth = 1;
		int start = i;
		for (i = i + 1; i < contents.length(); i++) {
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

		cout << innerTypeContents;

		originalContents = contents;
		contents = contents.substr(0, typeStart) + contents.substr(i + 1);


		if (innerTypeContents.find("{") != std::string::npos) {//This naive approach won't work, you can have both, you need to go line-by-line
			SourceTree* child = new SourceTree(innerTypeContents);
			children.push_back(child);
		}
		else {
			SourceTreeEndNode* child = new SourceTreeEndNode(innerTypeContents);
			endNodes.push_back(child);
		}
	}
}
