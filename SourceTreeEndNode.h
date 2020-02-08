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
				std::string original_contents;
				std::string identifier;
				std::string type;
				std::list<std::string> type_parameters;
			private:
				void Parse();
			};

		}
	}
}