#pragma once

#include <filesystem>
#include <span>

#include "Data.hpp"
#include "ErrorOr.hpp"
#include "json-parser/JSONValue.hpp"

#define EXIT_TEK 84

#define CONFIG_FILE Config::File::instance()

namespace Config {

  class File : public Data::Singleton<File> {
  public:
    friend class Data::Singleton<File>;
    auto init() -> MaybeError;

    [[nodiscard]] auto getIngredientsIds() const -> const Data::Ids &;

    [[nodiscard]] auto getRecipesIds() const -> const Data::Ids &;

    [[nodiscard]] auto getRecipesByIds() const -> const Data::RecipeBook &;

  private:
    File() = default;
    ~File() = default;

    auto parse_recipe(const std::string &key, const JSON::JSONValue &value)
      -> MaybeError;
    auto parse(const std::filesystem::path &path) -> MaybeError;

    std::unique_ptr<Data::Ids> _ingredientsIds;
    std::unique_ptr<Data::Ids> _recipesIds;
    Data::RecipeBook _recipesByIds;
  };

  struct Params {
    double multiplier;
    size_t cook;
    std::chrono::milliseconds time;
  };
}  // namespace Config
