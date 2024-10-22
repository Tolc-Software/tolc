#include "Helpers/Utils/combine.hpp"
#include <string>
#include <vector>

namespace Helpers::Utils {
	std::vector<std::string> combine(std::vector<std::string> const& v0, std::vector<std::string> const& v1) {
	    std::vector<std::string> combined;
	    combined.reserve(v0.size() + v1.size());
	    for (auto const& v : {v0, v1}) {
		    for (auto const& e : v) {
			    combined.push_back(e);
		    }
	    }
	    return combined;
    }
}
