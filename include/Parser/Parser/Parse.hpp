#pragma once

#include "IR/ir.hpp"
#include "Parser/Config.hpp"
#include <filesystem>
#include <string>

namespace Parser {
/**
* @brief: Parse the input file as C++ code and return the intermediate representation.
*
* @param: std::filesystem::path const& filename
*
* @return: IR::Namespace - The global namespace
*/
std::optional<IR::Namespace>
parseFile(std::filesystem::path const& filename,
          Parser::Config const& config = Parser::Config());

/**
* @brief: Parse the input as C++ code and return the intermediate representation.
*
* @param: std::string const& code
*
* @return: IR::Namespace - The global namespace
*/
std::optional<IR::Namespace>
parseString(std::string const& code,
            Parser::Config const& config = Parser::Config());
}    // namespace Parser
