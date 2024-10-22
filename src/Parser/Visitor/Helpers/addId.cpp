#include "Visitor/Helpers/addId.hpp"
#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <set>

namespace Visitor::Helpers {
void addIdToFunction(IRProxy::Function& f, IRProxy::IRData& data) {
	f.m_id = data.m_dependencyMap.size();
	data.m_dependencyMap.push_back(std::set<size_t>());
}

void addIdToClass(IRProxy::Struct& s, IRProxy::IRData& data) {
	s.m_id = data.m_dependencyMap.size();
	data.m_dependencyMap.push_back(std::set<size_t>());
	// Since this can be a dependency, remember its id
	data.m_idMap[s.m_fullyQualifiedName] = s.m_id;
}

void addIdToVariable(IR::Variable& v, IRProxy::IRData& data) {
	v.m_id = data.m_dependencyMap.size();
	data.m_dependencyMap.push_back(std::set<size_t>());
}

void addIdToEnum(IRProxy::Enum& e, IRProxy::IRData& data) {
	e.m_id = data.m_dependencyMap.size();
	data.m_dependencyMap.push_back(std::set<size_t>());
	// Since this can be a dependency, remember its id
	data.m_idMap[e.m_fullyQualifiedName] = e.m_id;
}

}    // namespace Visitor::Helpers
