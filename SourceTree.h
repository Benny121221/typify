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
				std::string original_contents;
				std::list<SourceTree*> children;
				std::list<SourceTreeEndNode*> end_nodes;
				std::string structure_identifier;
				std::string structure_type;
			private:
				void ParseChildren();
				void ParseInnards(int* pi, bool is_interface, std::string structure_type);
			};

		}
	}
}
