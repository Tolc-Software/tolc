#pragma once


#include "Objc/Conversions/conversion.hpp"
#include <string>

namespace Objc::Conversions {

Conversion addNamespace(Conversion c, std::string const& ns);

}    // namespace Objc::Conversions
