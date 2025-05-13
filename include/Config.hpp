#pragma once

#include <filesystem>

#include "Data.hpp"
#include "ErrorOr.hpp"
#include "json-parser/JSONValue.hpp"

#define EXIT_TEK 84

#define CONFIG_FILE Config::File::instance()

namespace Config {

  class File {
  public:
    static auto instance() -> File &;

    auto init() -> MaybeError;

    [[nodiscard]] auto getIngredientsIds() const -> const Data::Ids &;

    [[nodiscard]] auto getRecipesIds() const -> const Data::Ids &;

    [[nodiscard]] auto getRecipesByIds() const -> const Data::RecipeBook &;

    File(const File &) = delete;
    auto operator=(const File &) -> File & = delete;
    File(File &&) = delete;
    auto operator=(File &&) -> File & = delete;

  private:
    File() = default;
    ~File() = default;

    auto parse_recipe(
      const std::string &key,
      const std::shared_ptr<JSON::JSONValue> &value) -> MaybeError;
    auto parse(const std::filesystem::path &path) -> MaybeError;

    Data::Ids _ingredientsIds;
    Data::Ids _recipesIds;
    Data::RecipeBook _recipesByIds;
  };
}  // namespace Config
