#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "containers/BiMap.hpp"

namespace Data {

  using IdMapping = BiMap<size_t, std::string>;
  using RecipeBook = std::
    unordered_map<size_t, std::pair<size_t, std::vector<size_t>>>;
}  // namespace Data
