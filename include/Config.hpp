#pragma once

#include <chrono>
#include <memory>
#include <span>

#include "Data.hpp"
#include "ErrorOr.hpp"

namespace Config {

  struct Params {
    double multiplier;
    std::uint8_t cook;
    std::chrono::milliseconds time;
    static auto init(int argc, std::span<char *> argv) -> ErrorOr<Params>;
  };

  struct FileConfig {
    std::unique_ptr<Data::Ids> ingredientsIds;
    std::unique_ptr<Data::Ids> recipesIds;
    Data::RecipeBook recipesByIds;
    static auto init() -> ErrorOr<FileConfig>;
  };
}  // namespace Config
