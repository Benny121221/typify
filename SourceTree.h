#pragma once

#include "SourceTreeEndNode.h"
#include <string>
#include <list>
#include <map>

namespace Where1 {
	namespace typify {
		namespace Parsing {
			class SourceTree {
			public:
				SourceTree(std::string contents);
				std::string contents;
				std::string originalContents;
				std::list<SourceTree*> children;
				std::list<SourceTreeEndNode*> endNodes;
				std::string interfaceIdentifier;
			private:
				void ParseChildren();
				void ParseInnards(int* pi, bool is_interface);
			};

		}
	}
}
