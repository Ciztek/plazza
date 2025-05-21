#include "Config.hpp"
#include "ErrorOr.hpp"
#include "json-parser/JSONParser.hpp"
#include "json-parser/JSONValue.hpp"

namespace Config {
  namespace {
    auto parseRecipe(
      FileConfig &conf,
      const std::string &key,
      const JSON::JSONValue &value) -> MaybeError
    {
      conf.recipesIds->add(key);
      size_t recipe_id = TRY(conf.recipesIds->lookup(key));

      std::vector<size_t> recipeContent;

      JSON::JSONObject recipe_data = TRY(value.get<JSON::JSONObject>());
      JSON::JSONArray recipe_array = TRY(
        recipe_data["ingredients"].get<JSON::JSONArray>());
      double time_value = TRY(recipe_data["cooking_time"].get<double>());

      for (const auto &item: recipe_data)
        if (item.first != "ingredients" && item.first != "cooking_time")
          return Error("Invalid recipe key");

      if (recipe_array.empty())
        return Error("Recipe array is empty");

      for (const auto &item: recipe_array) {
        const std::string &ingredient = TRY(item.get<std::string>());
        size_t ingredient_id = TRY(conf.ingredientsIds->lookup(ingredient));
        if (ingredient_id == conf.ingredientsIds->size())
          return Error("Ingredient not found");
        recipeContent.push_back(ingredient_id);
      }

      if (time_value <= 0)
        return Error("Recipe time must be positive");

      conf.recipesByIds[recipe_id] = std::make_pair(time_value, recipeContent);
      return Nil{};
    }

    auto
    parse(FileConfig &conf, const std::filesystem::path &path) -> MaybeError
    {
      auto json = TRY(JSON::Parser::load_from_file(path));
      auto ingredients_array = TRY(json.get<JSON::JSONArray>("ingredients"));
      auto recipes_object = TRY(json.get<JSON::JSONObject>("recipes"));

      for (const auto &item: TRY(json.get<JSON::JSONObject>()))
        if (item.first != "ingredients" && item.first != "recipes")
          return Error("Invalid key in JSON");

      if (ingredients_array.empty())
        return Error("Ingredients array is empty");
      if (recipes_object.empty())
        return Error("Recipes object is empty");

      conf.ingredientsIds = std::
        make_unique<Data::Ids>(ingredients_array.size());
      conf.recipesIds = std::make_unique<Data::Ids>(recipes_object.size());

      for (const auto &item: ingredients_array) {
        const auto ingredient = TRY(item.get<std::string>());
        auto id = conf.ingredientsIds->lookup(ingredient);
        if (id.has_value())
          return Error("Duplicate ingredient found");
        conf.ingredientsIds->add(ingredient);
      }

      for (const auto &[key, value]: recipes_object)
        TRY(parseRecipe(conf, key, value));

      return Nil{};
    }
  }  // namespace

  auto Init::fileConf() -> ErrorOr<FileConfig>
  {
    FileConfig conf;

    auto conf_parsing = parse(conf, "config.json");
    if (conf_parsing.is_error())
      MUST(parse(conf, "default.json"), "Default.json not found or invalid");
    return conf;
  }
}  // namespace Config
