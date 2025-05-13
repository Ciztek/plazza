#pragma once

#include "Data.hpp"
#include "ErrorOr.hpp"

#define EXIT_TEK 84

namespace Config {
  static Data::Ids ingredientsIds;
  static Data::Ids recipesIds;
  static Data::RecipeBook recipesByIds;
  auto init() -> MaybeError;
}  // namespace Config
