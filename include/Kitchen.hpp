#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <map>

#include "Pizza.hpp"

namespace Plazza {

  class Kitchen {
  public:
    Kitchen() = default;

    Kitchen(const Kitchen &) = delete;
    Kitchen(Kitchen &&) = delete;
    auto operator=(const Kitchen &) -> Kitchen & = delete;
    auto operator=(Kitchen &&) -> Kitchen & = delete;

    void cook(Pizza &pizza);

    ~Kitchen() = default;

  private:
    class Fridge {
    public:
      enum class Ingredient : uint8_t {
        ChiefLove = 0,
        Dough,
        Eggplant,
        GoatCheese,
        Gruyere,
        Ham,
        Mushrooms,
        Steak,
        Tomato,
        COUNT
      };
      Fridge();
      ~Fridge() = default;

      [[nodiscard]] auto get(Ingredient ing) const -> uint8_t;
      void consume(Ingredient ing, uint8_t amount = 1);
      void refill(Ingredient ing, uint8_t amount = MAX_CAPACITY);

      static constexpr uint8_t MAX_CAPACITY = 5;

    private:
      std::array<
        std::atomic<uint8_t>,
        static_cast<std::size_t>(Ingredient::COUNT)>
        _stock;
    };

    Fridge _fridge;

    static constexpr uint8_t MAX_INGREDIENTS =
      static_cast<size_t>(Fridge::Ingredient::COUNT) * Fridge::MAX_CAPACITY;

    using IngredientList = std::array<Fridge::Ingredient, MAX_INGREDIENTS>;
    using RecipeBook = std::map<Pizza::Type, IngredientList>;
    static const RecipeBook _recipes;
  };
}  // namespace Plazza
