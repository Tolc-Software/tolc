#pragma once

#include "Objc/Proxy/type.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <string>

namespace Objc::Builders {
Objc::Proxy::Type buildType(IR::Type const& type, Objc::Cache& cache);
}
