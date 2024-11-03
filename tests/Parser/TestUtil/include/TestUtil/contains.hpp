#pragma once

#include <iostream>
#include <string>

namespace TestUtil {
	bool contains(std::string const& s, std::string const& sub) {
	    if (s.find(sub) != std::string::npos) {
		    return true;
	    }
	    std::cout << "Returned false: contains(\"\n"
	              << s << "\n\", \"\n"
	              << sub << "\n\")" << '\n';
	    return false;
    }
}
