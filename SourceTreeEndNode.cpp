#include "SourceTreeEndNode.h"
#include <iostream>
#include <string>

using namespace Where1::typify::Parsing;
using namespace std;

SourceTreeEndNode::SourceTreeEndNode(string contents) : contents(contents), original_contents(contents)
{
	is_array = false;
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

		string std_type_string = split_contents[0];
		if (std_type_string.find('<') != std::string::npos) {
			int k = std_type_string.find('<') + 1;
			int l = k;
			int depth = 1;
			int maxDepth = depth;

			while (depth != 0) {
				if (std_type_string[l] == '<') {
					depth++;
					maxDepth = maxDepth > depth ? maxDepth : depth;
				}

				if (std_type_string[l] == '>') {
					depth--;
					if (depth == 0) {
						break;
					}
				}
				l++;
			}
			int type_declaration_end = l;

			string type_declaration_temp = std_type_string.substr(k, type_declaration_end - k);
			
			int depth2 = 0;
			int last_splut_token = 0;//splut is past tense of split now, not splitted
			for(int m=0; m<type_declaration_temp.length(); m++) {
				switch (type_declaration_temp.at(m)) {
				case '<':
					depth2++;
					break;
				case '>':
					depth2--;
					if (depth2 == 0) {
						type_parameters.push_back(type_declaration_temp.substr(last_splut_token, m - last_splut_token + 1));
						last_splut_token = m;
					}
					break;
				case ',':
					if (depth2 == 0) {
						type_parameters.push_back(type_declaration_temp.substr(last_splut_token, m - last_splut_token));
						last_splut_token = m + 1;
					}
				}
			}

		}

		for (int k = 0; true; k++) {
			if (split_contents[0][k] == '\0' || split_contents[0][k] == '<') {
				break;
			}

			if (split_contents[0][k] == '[' && split_contents[0][k + 1] == ']') {
				is_array = true;
				split_contents[0][k] = '\0';//You can get away with this truncation because this has to be at end of segment
				break;
			}
		}

		type = split_contents[0];
		identifier = split_contents[1];
	}

	contents = "";
}