#include "Template/cache.hpp"
#include <algorithm>
#include <clang/AST/DeclCXX.h>
#include <string_view>
#include <vector>

namespace Template {

clang::CXXRecordDecl* Cache::findStruct(std::string const& strct) {
	auto s = std::find_if(m_structs.begin(),
	                      m_structs.end(),
	                      [strct](auto const& templateStruct) {
		                      return templateStruct->getNameAsString() == strct;
	                      });
	return s == m_structs.end() ? nullptr : *s;
}
}    // namespace Template
