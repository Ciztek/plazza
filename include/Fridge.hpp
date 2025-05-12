#pragma once

#include <array>
#include <atomic>
#include <cstdint>

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
  void refill(Ingredient ing, uint8_t amount = defaultStock);

private:
  static constexpr uint8_t defaultStock = 5;
  std::array<std::atomic<uint8_t>, static_cast<std::size_t>(Ingredient::COUNT)>
    _stock;
};
