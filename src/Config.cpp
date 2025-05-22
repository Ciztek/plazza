#include <chrono>
#include <cmath>
#include <cstddef>

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
      auto recipe_id = TRY(conf.recipesIds->lookup(key));

      std::vector<size_t> recipeContent;

      auto recipe_data = TRY(value.get<JSON::JSONObject>());
      auto recipe_array = TRY(
        recipe_data["ingredients"].get<JSON::JSONArray>());
      auto time_value = TRY(recipe_data["cooking_time"].get<double>());

      for (const auto &[dataKey, _]: recipe_data)
        if (dataKey != "ingredients" && dataKey != "cooking_time")
          return Error("Invalid recipe key");

      if (recipe_array.empty())
        return Error("Recipe array is empty");

      for (const auto &item: recipe_array) {
        auto ingredient = TRY(item.get<std::string>());
        auto ingredient_id = TRY(conf.ingredientsIds->lookup(ingredient));
        recipeContent.push_back(ingredient_id);
      }

      if (time_value <= 0)
        return Error("Recipe time must be positive");
      double intPart;
      // Violent integer check, I don't use std::floor because floating point
      // error
      if (std::fabs(std::modf(time_value, &intPart))
          >= std::numeric_limits<double>::epsilon())
        return Error("Recipe time must be an integer");
      conf.recipesByIds[recipe_id] = std::make_pair(time_value, recipeContent);
      return Nil{};
    }

    auto
    parse(FileConfig &conf, const std::filesystem::path &path) -> MaybeError
    {
      auto json = TRY(JSON::Parser::load_from_file(path));
      auto ingredients_array = TRY(json.get<JSON::JSONArray>("ingredients"));
      auto recipes_object = TRY(json.get<JSON::JSONObject>("recipes"));

      for (const auto &[key, _]: TRY(json.get<JSON::JSONObject>()))
        if (key != "ingredients" && key != "recipes")
          return Error("Invalid key in JSON");

      if (ingredients_array.empty())
        return Error("Ingredients array is empty");
      if (recipes_object.empty())
        return Error("Recipes object is empty");

      conf.ingredientsIds = std::
        make_unique<Data::Ids>(ingredients_array.size());
      conf.recipesIds = std::make_unique<Data::Ids>(recipes_object.size());

      for (const auto &item: ingredients_array) {
        auto ingredient = TRY(item.get<std::string>());
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

  namespace {

#define IS_ONLY(str, valid) (strspn(str, valid) == strlen(str))

    auto validateIntArgs(const char *arg) -> ErrorOr<int>
    {
      if (!IS_ONLY(arg, "0123456789"))
        return Error("Time parameter is not a positive integer");
      auto value = std::stoul(arg, nullptr, 10);
      if (value == 0)
        return Error("Time parameter cannot be 0");
      return value;
    }

    auto validateDoubleArgs(const char *arg) -> ErrorOr<double>
    {
      size_t idx = 0;
      double value = 0;

      try {
        value = std::stod(arg, &idx);
      } catch (...) {
        return Error("Multiplier parameter is not a double");
      }
      if (idx != strlen(arg))
        return Error("Multiplier parameter is not a double");
      if (value <= 0)
        return Error("Multiplier parameter cannot be 0 or negative");
      return value;
    }

#undef IS_ONLY

  }  // namespace

  auto Init::argConf(int argc, std::span<char *> argv) -> ErrorOr<Params>
  {
    Config::Params params;
    if (argc != 3)
      return Error("Not enough arguments");

    params.multiplier = TRY(validateDoubleArgs(argv[0]));
    params.cook = std::clamp(TRY(validateIntArgs(argv[1])), 1, 255);
    params.time = std::chrono::milliseconds(TRY(validateIntArgs(argv[2])));

    return params;
  }
}  // namespace Config
