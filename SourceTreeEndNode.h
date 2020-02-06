#pragma once

#include <string>
#include <list>
#include <map>

namespace Where1 {
	namespace typify {
		namespace Parsing {
			class SourceTreeEndNode {
			public:
				SourceTreeEndNode(std::string contents);
				std::string contents;
				std::string originalContents;
				std::string identifier;
				std::string type;
			private:
				void Parse();
			};

		}
	}
}