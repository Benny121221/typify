#include "SourceTreeEndNode.h"
#include <iostream>
#include <string>

using namespace Where1::typify::Parsing;
using namespace std;

SourceTreeEndNode::SourceTreeEndNode(string contents) : contents(contents)
{
	Parse();
}

void SourceTreeEndNode::Parse() {
	
}