#include "Frontend/Objc/frontend.hpp"
#include "Objc/Builders/moduleFileBuilder.hpp"
#include "Objc/Proxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Frontend::Objc {

std::optional<std::vector<std::pair<std::filesystem::path, std::string>>>
createModule(IR::Namespace const& rootNamespace,
             std::string const& moduleName) {
	std::vector<std::pair<std::filesystem::path, std::string>> out;
	if (auto maybeObjcFile =
	        ::Objc::Builders::buildModuleFile(rootNamespace, moduleName)) {
		auto& objcFile = maybeObjcFile.value();

		out.push_back(std::make_pair(objcFile.getObjcHeaderFile(),
		                             objcFile.getObjcHeader()));
		out.push_back(std::make_pair(objcFile.getObjcSourceFile(),
		                             objcFile.getObjcSource()));
		return out;
	}
	return std::nullopt;
}

}    // namespace Frontend::Objc
