#include <filesystem>

#include "Data.hpp"
#include "ErrorOr.hpp"
#include "Plazza.hpp"
#include "json-parser/JSONParser.hpp"
#include "json-parser/JSONValue.hpp"

namespace {

  auto parse_recipe(
    const std::string &key,
    const std::shared_ptr<JSON::JSONValue> &value) -> MaybeError
  {
    Config::recipesIds.add(key);
    size_t recipe_id = Config::recipesIds.lookup(key);

    std::vector<size_t> recipeContent;

    JSON::JsonObject recipe_data = TRY(value->get<JSON::JsonObject>());
    JSON::JsonArray recipe_array = TRY(
      recipe_data["ingredients"]->get<JSON::JsonArray>());
    double time_value = TRY(recipe_data["cooking_time"]->get<double>());

    if (recipe_array.empty())
      return Error("Recipe array is empty");
    for (const auto &item: recipe_array) {
      const std::string &ingredient = TRY(item->get<std::string>());
      size_t ingredient_id = Config::ingredientsIds.lookup(ingredient);
      if (ingredient_id == Config::ingredientsIds.size())
        return Error("Ingredient not found");
      recipeContent.push_back(ingredient_id);
    }
    if (time_value <= 0)
      return Error("Recipe time must be positive");
    Config::recipesByIds[recipe_id] = std::
      make_pair(time_value, recipeContent);
    return Nil{};
  }

  auto parse(const std::filesystem::path &path) -> MaybeError
  {
    JSON::JSONValue json = TRY(JSON::Parser::load_from_file(path));
    JSON::JsonArray ingredients_array = TRY(json.get<JSON::JsonArray>(
      "ingredient"
      "s"));
    JSON::
      JsonObject recipes_object = TRY(json.get<JSON::JsonObject>("recipes"));

    if (ingredients_array.empty())
      return Error("Ingredients array is empty");
    if (recipes_object.empty())
      return Error("Recipes object is empty");

    Config::ingredientsIds.setSize(ingredients_array.size());
    for (const auto &item: ingredients_array) {
      const auto ingredient = TRY(item->get<std::string>());
      Config::ingredientsIds.add(ingredient);
    }

    Config::recipesIds.setSize(recipes_object.size());
    for (const auto &[key, value]: recipes_object)
      TRY(parse_recipe(key, value));
    return Nil{};
  }
}  // namespace

namespace Config {
  auto init() -> MaybeError
  {
    auto config_parsing = parse("config.json");
    if (config_parsing.is_error())
      MUST(parse("default.json"), "Default.json not found or invalid");
    return {};
  }
}  // namespace Config
