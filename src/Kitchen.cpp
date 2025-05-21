#include "Config.hpp"
#include "ErrorOr.hpp"

#include "Kitchen.hpp"

Plazza::Kitchen::
  Kitchen(const Config::FileConfig &conf, const Config::Params &params)
  : _conf(conf), _params(params)
{
}

auto Plazza::Kitchen::cook(Pizza &pizza) -> MaybeError
{
  auto recipe = _conf.recipesByIds.find(pizza.getType());
  if (recipe == _conf.recipesByIds.end())
    return Error("Recipe not found");

  auto ingredients = recipe->second.second;
  for (auto ing: ingredients)
    TRY(_fridge.consume(ing));
  pizza.setState(Pizza::State::COOKING);
  // TODO: Assign the pizza to a cook and simulate time
  return Nil{};
}

Plazza::Kitchen::Fridge::Fridge(const Config::FileConfig &conf)
{
  _stock = std::vector<std::atomic<uint8_t>>(conf.ingredientsIds->size());
  for (auto &ingredient: _stock)
    ingredient.store(MAX_CAPACITY, std::memory_order_release);
}

auto Plazza::Kitchen::Fridge::get(size_t ing) const -> uint8_t
{
  return _stock[ing].load(std::memory_order_acquire);
}

auto Plazza::Kitchen::Fridge::consume(size_t ing, uint8_t amount) -> MaybeError
{
  auto &val = _stock[ing];
  uint8_t current = val.load(std::memory_order_acquire);

  if (amount == 0)
    return Error("Amount must be greater than 0");
  do {
    if (current < amount)
      return Error("Not enough ingredients in the fridge");
  } while (!val.compare_exchange_weak(
    current,
    current - amount,
    std::memory_order_acquire,
    std::memory_order_relaxed));
  return Nil{};
}

auto Plazza::Kitchen::Fridge::refill(size_t ing, uint8_t amount) -> MaybeError
{
  _stock[ing].store(amount, std::memory_order_release);
  return Nil{};
}
