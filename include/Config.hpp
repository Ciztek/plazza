#pragma once

#include <chrono>
#include <memory>

#include "Data.hpp"
#include "ErrorOr.hpp"

#define EXIT_TEK 84

namespace Config {

  struct Params {
    double multiplier;
    size_t cook;
    std::chrono::milliseconds time;
  };

  struct FileConfig {
    std::unique_ptr<Data::Ids> ingredientsIds;
    std::unique_ptr<Data::Ids> recipesIds;
    Data::RecipeBook recipesByIds;
  };

  struct Init {
    static auto fileConf() -> ErrorOr<FileConfig>;
  };
}  // namespace Config
