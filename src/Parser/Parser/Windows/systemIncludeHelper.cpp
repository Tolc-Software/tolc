#include "Parser/Windows/systemIncludeHelper.hpp"
#include "Helpers/Utils/split.hpp"
#include <algorithm>
#include <ctre.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace Parser::Windows {

static constexpr auto versionPattern =
    ctll::fixed_string {"^(\\d+\\.)?(\\d+\\.)?(\\d+\\.)?(\\*|\\d+)$"};

constexpr auto matchVersion(std::string_view maybeVersion) noexcept {
	return ctre::match<versionPattern>(maybeVersion);
}

struct Version {
	// Ex: {1, 13, 114, 3} for version 1.13.114.3
	std::vector<int> versionNumber;

	bool operator<(const Version& other) const {
		for (size_t i = 0;
		     i < std::min(versionNumber.size(), other.versionNumber.size());
		     i++) {
			if (versionNumber[i] < other.versionNumber[i]) {
				return true;
			} else if (versionNumber[i] > other.versionNumber[i]) {
				return false;
			}
			// They are the same, go to the next number
		}
		// Same version number -> order doesn't matter
		return true;
	};

	std::string toString() const {
		return fmt::format("{}", fmt::join(versionNumber, "."));
	}
};

std::string getLatestVersionDirectory(std::string path) {
	std::set<Version> pathsWithVersions;
	if (std::filesystem::exists(path)) {
		for (auto const& p : std::filesystem::directory_iterator(path)) {
			auto currentPath = p.path();
			auto fn = currentPath.filename().string();
			if (auto version = matchVersion(fn); p.is_directory() && version) {
				Version v;
				// NOTE: get<0> returns the whole capture
				if (auto major = version.get<1>()) {
					v.versionNumber.push_back(std::stoi(major.to_string()));
				}
				if (auto minor = version.get<2>()) {
					v.versionNumber.push_back(std::stoi(minor.to_string()));
				}
				if (auto patch = version.get<3>()) {
					v.versionNumber.push_back(std::stoi(patch.to_string()));
				}
				if (auto prerelase = version.get<4>()) {
					v.versionNumber.push_back(std::stoi(prerelase.to_string()));
				}
				pathsWithVersions.insert(v);
			}
		}
	}

	// NOTE: This assumes that if there are multiple versions,
	//       they are all of the same type
	//       (i.e. they all include the same combination of {major, minor, patch, prerelease})
	return pathsWithVersions.empty() ?
	           path :
	           path + (*pathsWithVersions.rbegin()).toString();
}

/*
* Searches through path for versionPlaceHolder and switches it out for the largest version number available in the current filesystem
* If no version is found it just returns the input, removing any versionPlaceHolder
*/
std::string getLatestVersionFromPath(std::string path,
                                     std::string_view versionPlaceHolder) {
	auto splitPath = Helpers::Utils::split(path, versionPlaceHolder);

	// NOTE: Do not search for latest version in the last entry
	// Ex:
	//    C:/Hi/{versionPlaceHolder}/Bye
	//    Should only search for version in C:/Hi/
	//    Not in C:/Hi/{expandedVersion}/Bye
	std::string outPath = "";
	for (size_t i = 0; i < splitPath.size() - 1; i++) {
		outPath = getLatestVersionDirectory(outPath + splitPath[i]);
	}
	// Add in the last part of the path (in the above example "/Bye")
	outPath += splitPath.back();
	return outPath;
}

std::vector<std::string>
filterPathsWithLatestVersion(std::vector<std::string> const& potentialPaths,
                             std::string_view versionPlaceholder) {
	std::vector<std::string> expandedPaths;
	for (auto const& p : potentialPaths) {
		expandedPaths.push_back(
		    getLatestVersionFromPath(p, versionPlaceholder));
	}
	return expandedPaths;
}

std::vector<std::string>
appendSystemIncludes(std::vector<std::string> const& paths) {
	std::vector<std::string> includes;
	for (auto const& path : paths) {
		includes.push_back("-isystem" + path);
	}
	return includes;
}
}    // namespace Parser::Windows
