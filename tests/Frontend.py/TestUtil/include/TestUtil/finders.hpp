#pragma once

#include <IR/ir.hpp>
#include <algorithm>
#include <optional>
#include <string_view>
#include <vector>

namespace TestUtil {

template <typename IRType>
std::optional<IRType> findWithName(std::string_view name,
                                   std::vector<IRType> const& v) {
	if (auto it =
	        std::find_if(v.begin(),
	                     v.end(),
	                     [&name](auto const& ir) { return ir.m_name == name; });
	    it != v.end()) {
		return *it;
	}
	return {};
}

}    // namespace TestUtil
