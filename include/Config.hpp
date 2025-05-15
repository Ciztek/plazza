#pragma once

#include <filesystem>

#include "Data.hpp"
#include "ErrorOr.hpp"
#include "json-parser/JSONValue.hpp"

#define EXIT_TEK 84

#define CONFIG_FILE Config::File::instance()
#define CONFIG_ARGS Config::Args::instance()

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

    auto parse_recipe(
      const std::string &key,
      const std::shared_ptr<JSON::JSONValue> &value) -> MaybeError;
    auto parse(const std::filesystem::path &path) -> MaybeError;

    std::unique_ptr<Data::Ids> _ingredientsIds;
    std::unique_ptr<Data::Ids> _recipesIds;
    Data::RecipeBook _recipesByIds;
  };

  class Args : public Data::Singleton<Args> {
  public:
    friend class Data::Singleton<Args>;
    auto init(int argc, char *argv[]) -> MaybeError;

    [[nodiscard]] auto getMultiplier() const -> double;
    [[nodiscard]] auto getCook() const -> size_t;
    [[nodiscard]] auto getTime() const -> std::chrono::milliseconds;

  private:
    Args() = default;
    ~Args() = default;

    double _multiplier;
    size_t _cook;
    std::chrono::milliseconds _time;
  };
}  // namespace Config
