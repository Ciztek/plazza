#include "KitchenCatalog.hpp"
#include "json-parser/JSONParser.hpp"

#define MAX_CATALOG_ITEM_COUNT 255

namespace {
  auto populate_ingredients(JSON::JSONValue &ref_conf, KitchenCalatog &catalog)
    -> MaybeError
  {
    size_t ids = 0;
    auto ingredient_list = TRY(ref_conf.get<JSON::JSONArray>("ingredients"));

    MUST(
      ingredient_list.size() < MAX_CATALOG_ITEM_COUNT,
      "too many item in the catalog!");
    for (auto &item: ingredient_list) {
      auto ing_name = TRY(item.get<std::string>());
      if (catalog.ingredients.contains_value(ing_name)) {
        Log::warn << "duplicated ingredient name: " + ing_name;
        continue;
      }
      TRY(catalog.ingredients.insert(ids, ing_name));
      ids++;
    }

    return {};
  }

  auto populate_recipe_ingredients(
    JSON::JSONArray &ingredient_list,
    KitchenCalatog &catalog,
    Recipe &current) -> MaybeError
  {
    for (auto &item: ingredient_list) {
      auto ing_name = TRY(item.get<std::string>());
      auto ing_id = TRY(catalog.ingredients.at_value(ing_name));

      current.ingredients.emplace_back(ing_id);
    }

    return {};
  }

  auto populate_recipes(JSON::JSONValue &ref_conf, KitchenCalatog &catalog)
    -> MaybeError
  {
    size_t ids = 0;

    // Note on pragma disgonstic ignored
    // Fix false positive of `-Wfree-nonheap-object`
    // caused by `[[nodiscard]] auto get() const -> ErrorOr<T>` in
    // JSONValue.hpp See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=108088

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
    auto recipes_list = TRY(ref_conf.get<JSON::JSONObject>("recipes"));

#pragma GCC diagnostic pop

    for (const auto &[name, details]: recipes_list) {
      Recipe current;

      current.cook_time = TRY(details.get<double>("cooking_time"));
      if (catalog.recipes.contains_value(name)) {
        Log::warn << "duplicated recipe name: " + name;
        continue;
      }

      auto ingredient_list = TRY(details.get<JSON::JSONArray>("ingredients"));
      TRY(populate_recipe_ingredients(ingredient_list, catalog, current));
      TRY(catalog.recipes.insert(ids, name));
      catalog.details.push_back(current);
      ids++;
    }

    return {};
  }
}  // namespace

auto KitchenCalatog::load_from_file(const fs::path &filepath)
  -> ErrorOr<KitchenCalatog>
{
  KitchenCalatog catalog;

  auto parsed = TRY(JSON::Parser::load_from_file(filepath));

  TRY(populate_ingredients(parsed, catalog));
  TRY(populate_recipes(parsed, catalog));
  return catalog;
}
