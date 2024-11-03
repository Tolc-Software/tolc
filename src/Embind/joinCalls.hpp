#pragma once

#include <string>
#include <vector>

namespace Embind {

template <typename EmbindObject>
std::string joinEmbind(std::vector<EmbindObject> const& objects) {
  std::string out;
  for (auto const& o : objects) {
    out += o.getEmbind();
  }
  return out;
}

template <typename EmbindObject>
std::string joinEmbind(std::string const& prefix,
                       std::vector<EmbindObject> const& objects) {
  std::string out;
  for (auto const& o : objects) {
    out += o.getEmbind(prefix);
  }
  return out;
}

template <typename EmbindObject>
std::string joinPreJS(std::string const& prefix,
                      std::vector<EmbindObject> const& objects) {
  std::string out;
  for (auto const& o : objects) {
    out += o.getPreJS(prefix);
  }
  return out;
}

template <typename EmbindObject>
std::string joinPreJS(std::string const& prefix,
                      std::vector<EmbindObject> const& objects,
                      std::vector<std::string>& names) {
  std::string out;
  for (auto const& o : objects) {
    out += o.getPreJS(prefix, names);
  }
  return out;
}

template <typename EmbindObject>
std::string joinGlobalPreJS(std::string const& prefix,
                            std::vector<EmbindObject> const& objects,
                            std::vector<std::string>& names) {
  std::string out;
  for (auto const& o : objects) {
    out += o.getGlobalPreJS(prefix, names);
  }
  return out;
}

}    // namespace Embind
