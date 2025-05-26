#pragma once

#include "Data.hpp"
#include "ErrorOr.hpp"

namespace Config {

  struct FileConfig {
    Data::IdMapping ingredientsIds;
    Data::IdMapping recipesIds;
    Data::RecipeBook recipesByIds;
    static auto init() -> ErrorOr<FileConfig>;
  };
}  // namespace Config
