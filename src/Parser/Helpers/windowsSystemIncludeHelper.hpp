#pragma once
/*
* At the time of writing this, this file is only used on Windows to dynamically find the latest version of the standard library on the users machine.
* On all other OS's this code does not do anything
*/

#include <string>
#include <string_view>
#include <vector>

namespace Helpers {
/**
* @brief: Go through potentialPaths and look for them in the filesystem expanding versionPlaceholder to the latest version.
*         Ex:
*           versionPlaceholper = {LATEST_VERSION}
*           potentialPaths = {C:\Program Files\MSVC\{LATEST_VERSION}\include}
*           On the filesystem there exists: C:\Program Files\MSVC\10 and C:\Program Files\MSVC\11
*           So the return will be {C:\Program Files\MSVC\10 and C:\Program Files\MSVC\11}
*
* @param: std::vector<std::string> const& potentialPaths,
*         std::string_view versionPlaceholder);
*
* @return: std::vector<std::string>
*/
std::vector<std::string> filterExistingPathsWithLatestVersion(
    std::vector<std::string> const& potentialPaths,
    std::string_view versionPlaceholder);

/**
* @brief: Go through paths and append "-isystem" before each string
*
* @param: std::vector<std::string> const& paths,
*
* @return: std::vector<std::string>
*/
std::vector<std::string>
appendSystemIncludes(std::vector<std::string> const& paths);
}    // namespace Helpers
