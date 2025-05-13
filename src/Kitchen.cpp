#include "Config.hpp"

#include "Kitchen.hpp"

using namespace Plazza;

void Kitchen::cook(Pizza &pizza)
{
  auto recipe = CONFIG_FILE.getRecipesByIds().find(pizza.getType());
  if (recipe == CONFIG_FILE.getRecipesByIds().end())
    throw std::runtime_error("Unknown pizza type.");
  auto ingredients = recipe->second.second;
  for (auto ing: ingredients)
    _fridge.consume(ing);
  pizza.setState(Pizza::State::COOKING);
  // TODO: Assign the pizza to a cook and simulate time
}

Kitchen::Fridge::Fridge()
{
  _stock = std::
    vector<std::atomic<uint8_t>>(CONFIG_FILE.getIngredientsIds().size());
  for (auto &ingredient: _stock)
    ingredient.store(MAX_CAPACITY, std::memory_order_release);
}

auto Kitchen::Fridge::get(size_t ing) const -> uint8_t
{
  return _stock[ing].load(std::memory_order_acquire);
}

void Kitchen::Fridge::consume(size_t ing, uint8_t amount)
{
  auto &val = _stock[ing];
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

void Kitchen::Fridge::refill(size_t ing, uint8_t amount)
{
  _stock[ing].store(amount, std::memory_order_release);
}
