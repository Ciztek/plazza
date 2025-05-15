#include "Config.hpp"
#include <memory>
#include "json-parser/JSONParser.hpp"
#include "json-parser/JSONValue.hpp"

namespace Config {

  auto File::instance() -> File &
  {
    static File instance;
    return instance;
  }

  auto File::getIngredientsIds() const -> const Data::Ids &
  {
    return *_ingredientsIds;
  }

  auto File::getRecipesIds() const -> const Data::Ids &
  {
    return *_recipesIds;
  }

  auto File::getRecipesByIds() const -> const Data::RecipeBook &
  {
    return _recipesByIds;
  }

  /*-------------------------------------------------------------------------*/
  /*-------------------------------------------------------------------------*/
  /*-------------------------------------------------------------------------*/

  auto File::parse_recipe(
    const std::string &key,
    const std::shared_ptr<JSON::JSONValue> &value) -> MaybeError
  {
    _recipesIds->add(key);
    size_t recipe_id = TRY(_recipesIds->lookup(key));

    std::vector<size_t> recipeContent;

    JSON::JsonObject recipe_data = TRY(value->get<JSON::JsonObject>());
    JSON::JsonArray recipe_array = TRY(
      recipe_data["ingredients"]->get<JSON::JsonArray>());
    double time_value = TRY(recipe_data["cooking_time"]->get<double>());

    for (const auto &item: recipe_data)
      if (item.first != "ingredients" && item.first != "cooking_time")
        return Error("Invalid recipe key");

    if (recipe_array.empty())
      return Error("Recipe array is empty");

    for (const auto &item: recipe_array) {
      const std::string &ingredient = TRY(item->get<std::string>());
      size_t ingredient_id = TRY(_ingredientsIds->lookup(ingredient));
      if (ingredient_id == _ingredientsIds->size())
        return Error("Ingredient not found");
      recipeContent.push_back(ingredient_id);
    }

    if (time_value <= 0)
      return Error("Recipe time must be positive");

    _recipesByIds[recipe_id] = std::make_pair(time_value, recipeContent);
    return Nil{};
  }

  auto File::parse(const std::filesystem::path &path) -> MaybeError
  {
    JSON::JSONValue json = TRY(JSON::Parser::load_from_file(path));
    JSON::JsonArray ingredients_array = TRY(json.get<JSON::JsonArray>(
      "ingredient"
      "s"));
    JSON::
      JsonObject recipes_object = TRY(json.get<JSON::JsonObject>("recipes"));

    for (const auto &item: TRY(json.get<JSON::JsonObject>()))
      if (item.first != "ingredients" && item.first != "recipes")
        return Error("Invalid key in JSON");

    if (ingredients_array.empty())
      return Error("Ingredients array is empty");
    if (recipes_object.empty())
      return Error("Recipes object is empty");

    _ingredientsIds = std::make_unique<Data::Ids>(ingredients_array.size());
    _recipesIds = std::make_unique<Data::Ids>(recipes_object.size());

    for (const auto &item: ingredients_array) {
      const auto ingredient = TRY(item->get<std::string>());
      auto id = _ingredientsIds->lookup(ingredient);
      if (id.has_value())
        return Error("Duplicate ingredient found");
      _ingredientsIds->add(ingredient);
    }

    for (const auto &[key, value]: recipes_object)
      TRY(parse_recipe(key, value));

    return Nil{};
  }

  auto File::init() -> MaybeError
  {
    auto config_parsing = parse("config.json");
    if (config_parsing.is_error())
      MUST(parse("default.json"), "Default.json not found or invalid");
    return {};
  }

}  // namespace Config
