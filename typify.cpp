// typify.cpp : Defines the entry point for the application.
//

#include "typify.h"
#include "SourceTree.cpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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
		/*else if (input[i] == '\n') { This will probably mess shit up
			input.replace(i, 1, " ");
			mutated = true;
		}*/

		if (mutated) {
			i--;//Prevents skipping because of deletions
		}
	}

	string* ptr = new string[input.length()];
	*ptr = input;
	return ptr;

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

	cout << input << endl << endl << endl;

	input = *trimWhitespace(input);
	cout << input << endl << endl << endl;

	SourceTree asTree = SourceTree::SourceTree(input);

	return 0;
}
