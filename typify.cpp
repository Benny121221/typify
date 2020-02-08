// typify.cpp : Defines the entry point for the application.
//

#include "SourceTree.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;


unordered_map<string, string> typeTranslationTable({
	{"byte","number"},
	{"sbyte","number"},
	{"short","number"},
	{"ushort","number"},
	{"int","number"},
	{"uint","number"},
	{"long","number"},
	{"ulong","number"},
	{"float","number"},
	{"double","number"},
	{"DateTime","Date"},
	{"DateTimeOffset","Date"},
	});

string* trimWhitespace(string input) {
	for (int i = 0; i < input.length(); i++) {
		bool mutated = false;

		if (input[i] == ' ') {
			if (i + 1 < input.length() && input[i + 1] == ' ') {
				input.erase(i, 1);
				mutated = true;
			}
		}
		else if (input[i] == '\t') {
			input.replace(i, 1, " ");
			mutated = true;
		}

		if (input[i] == '<') {
			int end = input.find('>', i);
			if (end != std::string::npos) {
				for (int j = 0; j < end - i; j++) {
					if (input[i + j] == ' ') {
						input.erase(i + j, 1);
						j--;//Prevent skipping due to deletions
						i--;
					}
				}
			}
		}
		if (mutated) {
			i--;//Prevents skipping because of deletions
		}
	}

	string* ptr = new string[input.length()];
	*ptr = input;
	return ptr;
}

void printOutput(SourceTree* tree, bool root = true) {
	if (root) {
		cout << "interface " << tree->interface_identifier << "{" << endl;
	}

	for (SourceTree* curr : tree->children) {
		cout << "{" << endl;
		printOutput(curr, false);
		cout << "}" << endl;
	}

	for (SourceTreeEndNode* curr : tree->end_nodes) {
		if (curr->type != "") {
			string typescriptType = curr->type;
			if (typeTranslationTable.count(curr->type) != 0) {
				typescriptType = typeTranslationTable.at(curr->type);
			}

			if (typescriptType.find('<') != std::string::npos) {
				int start = typescriptType.find('<');
				int end = typescriptType.find('>', start);
				typescriptType.erase(start, end - start + 1);

				typescriptType += "<";
				for (string currTypeParam : curr->type_parameters) {
					string toAdd = currTypeParam;

					if (typeTranslationTable.count(currTypeParam) != 0) {
						toAdd = typeTranslationTable.at(currTypeParam);
					}
					typescriptType += toAdd + ",";
				}
				typescriptType.erase(typescriptType.length() - 1, 1);
				typescriptType += ">";

			}

			if (curr->is_array) {
				typescriptType += "[]";
			}


			cout << curr->identifier << " : " << typescriptType << ";" << endl;
		}
	}

	if (root) {
		cout << "}" << endl;
	}
}

int main()
{
	string input;
	string filepath;

	cout << "What is the filepath?" << endl;
	cin >> filepath;

	ifstream file;
	file.open(filepath);
	string line;
	while (getline(file, line))
	{
		input += line + '\n';//Reading line-by-line prematurely gets rid of newlines
	}

	file.close();

	input = *trimWhitespace(input);

	SourceTree asTree = SourceTree::SourceTree(input);


	cout << endl << endl << endl;
	printOutput(&asTree);

	cout << endl << endl << endl << "Hit any key to close" << endl;
	string wait;
	cin >> wait;

	return 0;
}
