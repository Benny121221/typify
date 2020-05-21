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
				for (int j = 0; j < end - i - 1; j++) {
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
		cout << tree->structure_type << " " << tree->structure_identifier << "{" << endl;
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
				int depth = 1;
				int seek = start + 1;
				while (true) {
					if (typescriptType.at(seek) == '<') {
						depth++;
					}

					if (typescriptType.at(seek) == '>') {
						depth--;
						if (depth == 0) {
							break;
						}
					}
					seek++;
				}

				int end = seek;
				typescriptType.erase(start, end - start + 1);

				typescriptType += "<";
				for (string currTypeParam : curr->type_parameters) {
					string toAdd = currTypeParam;
					string toAddTemp;

					bool isArray = false;
					if (currTypeParam.find("[]") == currTypeParam.length() - 2) {
						isArray = true;
						currTypeParam.erase(currTypeParam.length() - 2);
					}

					if (typeTranslationTable.count(currTypeParam) != 0) {
						toAdd = typeTranslationTable.at(currTypeParam);

						if (isArray) {
							toAdd += "[]";
						}
					}
					else if (currTypeParam.find('<') != std::string::npos) {
						toAdd = "";
						int depth = 0;
						int startComplexType = 0;
						int startInnerType = 0;
						for (int i = 0; i < currTypeParam.length(); i++) {
							if (currTypeParam.at(i) == '<') {
								depth++;
								bool isArray = false;
								toAddTemp = currTypeParam.substr(startComplexType, i - startComplexType);
								if (toAddTemp.find("[]") == toAddTemp.length() - 2) {
									isArray = true;
									toAddTemp.erase(toAddTemp.length() - 2);
								}
								if (typeTranslationTable.count(toAddTemp) != 0) {
									toAddTemp = typeTranslationTable.at(toAddTemp);
								}
								toAdd += toAddTemp;
								if (isArray) {
									toAdd += "[]";
								}

								startInnerType = i;
							}
							else if (currTypeParam.at(i) == '>') {
								depth--;
								bool isArray = false;
								toAddTemp = currTypeParam.substr(startInnerType, i - startInnerType);
								if (toAddTemp.at(0) == ',') {
									toAdd += ',';
									toAddTemp.erase(0, 1);
								}
								if (toAddTemp.find("[]") == toAddTemp.length() - 2) {
									isArray = true;
									toAddTemp.erase(toAddTemp.length() - 2);
								}
								if (typeTranslationTable.count(toAddTemp) != 0) {
									toAddTemp = typeTranslationTable.at(toAddTemp);
								}
								if (isArray) {
									toAdd += "[]";
								}

								toAdd += toAddTemp;
							}
							else if (currTypeParam.at(i) == ',') {
								if (depth == 0) {
									bool isArray = false;
									toAddTemp = currTypeParam.substr(startComplexType, i - startComplexType);
									if (typeTranslationTable.count(toAddTemp) != 0) {
										toAddTemp = typeTranslationTable.at(toAddTemp);
									}
									if (toAddTemp.find("[]") == toAddTemp.length() - 2) {
										isArray = true;
										toAddTemp.erase(toAddTemp.length() - 2);
									}
									if (isArray) {
										toAdd += "[]";
									}

									toAdd += toAddTemp;
									startComplexType = i;
								}
								else {
									int tempStart = startInnerType < startComplexType ? startComplexType : startInnerType;
									bool isArray = false;
									toAddTemp = currTypeParam.substr(tempStart, i - tempStart);
									if (toAddTemp.at(0) == '<') {
										toAdd += '<';
										toAddTemp.erase(0, 1);
									}
									if (toAddTemp.find("[]") == toAddTemp.length() - 2) {
										isArray = true;
										toAddTemp.erase(toAddTemp.length() - 2);
									}
									if (typeTranslationTable.count(toAddTemp) != 0) {
										toAddTemp = typeTranslationTable.at(toAddTemp);
									}
									if (isArray) {
										toAdd += "[]";
									}

									toAdd += toAddTemp;
									startInnerType = i;
								}
							}
						}
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

int main(int argc, char** argv)
{
	string input;
	string filepath;
	string out_file;
	
	if (argc > 1) {
		filepath = argv[1];
	}
	else {
		cout << "What is the filepath?" << endl;
		cin >> filepath;
	}


	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
			if (i + 1 < argc) {
				out_file = argv[i + 1];
				break;
			}
		}
	}

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


	
	if (out_file.empty()) {
		cout << endl << endl << endl;
		printOutput(&asTree);

		cout << endl << endl << endl << "Hit any key to close" << endl;
		string wait;
		cin >> wait;
	}
	else {
		freopen(out_file.c_str(), "a+", stdout);
		printOutput(&asTree);
	}

	return 0;
}
