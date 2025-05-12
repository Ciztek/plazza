#include <stdexcept>

#include "Fridge.hpp"

using namespace Plazza;

Fridge::Fridge()
{
  for (auto &ingredient: _stock)
    ingredient.store(defaultStock, std::memory_order_release);
}

auto Fridge::get(Ingredient ing) const -> uint8_t
{
  return _stock[static_cast<std::size_t>(ing)].load(std::memory_order_acquire);
}

void Fridge::consume(Ingredient ing, uint8_t amount)
{
  auto &val = _stock[static_cast<std::size_t>(ing)];
  uint8_t current = val.load(std::memory_order_acquire);
  if (current < amount)
    throw std::runtime_error("Not enough stock to consume.");
  while (!val.compare_exchange_weak(
    current,
    current - amount,
    std::memory_order_release,
    std::memory_order_acquire))
    ;  // Retry till sucess
}

void Fridge::refill(Ingredient ing, uint8_t amount)
{
  _stock[static_cast<std::size_t>(ing)].store(
    amount, std::memory_order_release);
}
