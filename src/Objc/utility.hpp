#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace Objc {

// A category is an extension of a class in Objective-C
std::string getCategoryName(char prefix,
                            std::string_view objcClass,
                            std::string_view name,
                            std::optional<size_t> overloadIndex = std::nullopt);

std::string wrapInInterface(std::string_view objcClass,
                            std::string_view category,
                            std::string_view code);

std::string wrapInImplementation(std::string_view objcClass,
                                 std::string_view category,
                                 std::string_view code);

// Get a documentation string if not empty
std::string getDocumentationString(std::string_view documentation);
}
