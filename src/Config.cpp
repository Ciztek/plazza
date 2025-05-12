#include <iostream>
#include <variant>

#include "Exception.hpp"
#include "Plazza.hpp"
#include "json/JSON.hpp"

namespace {
  void parse_ingredient(const Parser::JSON &json)
  {
    const auto &ingredients_array = json["\"ingredients\""];
    const auto *array_ptr = Parser::JSON::
      get_if_or_throw<Parser::JsonArray>(ingredients_array, "ingredients");
    for (const auto &item: *array_ptr) {
      const auto *ingredient_ptr = Parser::JSON::
        get_if_or_throw<std::string>(*item, "ingredients array item");
      std::cout << "Ingredient: " << *ingredient_ptr << "\n";
    }
  }

  void parse_recipe_content(
    const Parser::JsonObject &recipe_data,
    const std::string &key)
  {
    const auto time = recipe_data.at("\"cooking_time\"");
    if (!std::holds_alternative<double>(time->value))
      throw Parser::ParserException("Recipe time must be a double");
    const auto recipe_array_ptr = recipe_data.at("\"ingredients\"");
    if (!std::holds_alternative<Parser::JsonArray>(recipe_array_ptr->value))
      throw Parser::ParserException("Recipe must be an array");
    Parser::JsonArray recipe_array = std::
      get<Parser::JsonArray>(recipe_array_ptr->value);
    if (recipe_array.empty())
      throw Parser::ParserException("Recipe array is empty");
    std::cout << "Recipe: " << key << "\n";
    for (const auto &item: recipe_array) {
      const auto *ingredient_ptr = Parser::JSON::
        get_if_or_throw<std::string>(*item, "recipe." + key + " item");
      std::cout << "recipe ingredient: " << *ingredient_ptr << "\n";
    }
    std::cout << "Time: " << std::get<double>(time->value) << "\n";
  }

  void parse_recipe(const Parser::JSON &json)
  {
    const auto &recipe_object = json["\"recipes\""];
    const auto *recipe_ptr = Parser::JSON::
      get_if_or_throw<Parser::JsonObject>(recipe_object, "recipe");
    for (const auto &[key, value]: *recipe_ptr) {
      if (!std::holds_alternative<Parser::JsonObject>(value->value))
        throw Parser::ParserException("Recipe must be an object");
      Parser::JsonObject recipe_data = std::
        get<Parser::JsonObject>(value->value);
      parse_recipe_content(recipe_data, key);
    }
  }

  void parse_config(const std::filesystem::path &path)
  {
    Parser::JSON json(path);

    parse_ingredient(json);
    parse_recipe(json);
  }
}  // namespace

namespace Config {
  [[gnu::constructor]]
  void init()
  {
    try {
      parse_config("config.json");
    } catch (const Parser::FileException &e) {
      try {
        parse_config("default.json");
      } catch (const Parser::FileException &nested_e) {
        std::cerr << nested_e.what() << "\n";
        exit(EXIT_FAILURE);
      }
    } catch (const Parser::ParserException &e) {
      std::cerr << e.what() << "\n";
      exit(EXIT_FAILURE);
    } catch (const std::exception &e) {
      std::cerr << "An error occurred: " << e.what() << "\n";
      exit(EXIT_FAILURE);
    }
  }
}  // namespace Config
