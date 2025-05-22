#include <chrono>
#include <cmath>
#include <cstddef>

#include "Config.hpp"
#include "ErrorOr.hpp"
#include "json-parser/JSONParser.hpp"
#include "json-parser/JSONValue.hpp"

// Note on pragma disgonstic ignored
// Fix false positive of `-Wfree-nonheap-object`
// caused by `[[nodiscard]] auto get() const -> ErrorOr<T>` in JSONValue.hpp
// See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=108088

namespace Config {
  namespace {
    auto parseRecipe(
      FileConfig &conf,
      const std::string &key,
      const JSON::JSONValue &value) -> MaybeError
    {
      TRY(conf.recipesIds.insert(conf.recipesIds.size(), key));
      auto recipe_id = TRY(conf.recipesIds.at_value(key));

      std::vector<size_t> recipeContent;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
      auto recipe_data = TRY(value.get<JSON::JSONObject>());
#pragma GCC diagnostic pop
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
        auto ingredient_id = TRY(conf.ingredientsIds.at_value(ingredient));
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

    auto parse(const std::filesystem::path &path) -> ErrorOr<FileConfig>
    {
      FileConfig conf;

      auto json = TRY(JSON::Parser::load_from_file(path));
      auto ingredients_array = TRY(json.get<JSON::JSONArray>("ingredients"));

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
      auto recipes_object = TRY(json.get<JSON::JSONObject>("recipes"));

      for (const auto &[key, _]: TRY(json.get<JSON::JSONObject>()))
        if (key != "ingredients" && key != "recipes")
          return Error("Invalid key in JSON");
#pragma GCC diagnostic pop

      MUST(!ingredients_array.empty(), "Ingredients array is empty");
      MUST(!recipes_object.empty(), "Recipes object is empty");

      conf.ingredientsIds = Data::IdMapping(ingredients_array.size());
      conf.recipesIds = Data::IdMapping(recipes_object.size());
      conf.recipesByIds = Data::RecipeBook(recipes_object.size());

      for (const auto &item: ingredients_array) {
        auto ingredient = TRY(item.get<std::string>());

        if (conf.ingredientsIds.contains_value(ingredient))
          return Error("Duplicate ingredient name");
        TRY(
          conf.ingredientsIds.insert(conf.ingredientsIds.size(), ingredient));
      }

      for (const auto &[key, value]: recipes_object)
        TRY(parseRecipe(conf, key, value));

      return conf;
    }
  }  // namespace

  auto Config::FileConfig::init() -> ErrorOr<FileConfig>
  {
    auto conf = parse("config.json");
    if (!conf)
      conf = TRY(parse("default.json"));
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

  auto
  Config::Params::init(int argc, std::span<char *> argv) -> ErrorOr<Params>
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
