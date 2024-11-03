#pragma once

#include <set>
#include <vector>

namespace Embind::Helpers {
/**
* Add the contents of the vector to the set
*/
template <typename T>
void combine(std::set<T>& result, std::vector<T> const& toBeAdded) {
  for (auto const& v : toBeAdded) {
    if (result.find(v) == result.end()) {
      result.insert(v);
    }
  }
}
}    // namespace Embind::Helpers
