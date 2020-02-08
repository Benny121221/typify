#include "SourceTreeEndNode.h"
#include <iostream>
#include <string>

using namespace Where1::typify::Parsing;
using namespace std;

SourceTreeEndNode::SourceTreeEndNode(string contents) : contents(contents), original_contents(contents)
{
	Parse();
}

void SourceTreeEndNode::Parse() {
	char* contents_cstr = (char*)contents.c_str();//strtok mutates
	char* split_contents[2];

	char* ptemp = strtok(contents_cstr, " ");
	bool successful_write = false;

	int i = 0;
	while (ptemp != NULL) {
		if (i == 2) {
			throw new exception("Parsing tree end node found 3+ identifiers (type or otherwise)");
		}
		split_contents[i] = ptemp;

		i++;
		ptemp = strtok(NULL, " ");
		successful_write = true;
	}

	if (successful_write) {//Ensures empty lines are not parsed with unitialized funzies
		int j = 0;
		while (split_contents[1][j] != '\0') {
			if (split_contents[1][j] == ';') {
				split_contents[1][j] = '\0';//Semicolons anywhere other than at end of statement (or in string literal) is HIGHLY sus, so this is ok
				break;
			}
			j++;
		}

		type = split_contents[0];
		identifier = split_contents[1];
	}

	contents = "";
}