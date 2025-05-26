#pragma once

#include <filesystem>
#include <vector>

#include "ErrorOr.hpp"
#include "containers/BiMap.hpp"

namespace fs = std::filesystem;

struct Recipe {
  double cook_time;
  std::vector<size_t> ingredients;
};

struct KitchenCalatog {
  BiMap<size_t, std::string> ingredients;
  BiMap<size_t, std::string> recipes;
  std::vector<Recipe> details;

  static auto
  load_from_file(const fs::path &filepath) -> ErrorOr<KitchenCalatog>;
};
