#include <stdexcept>

#include "Kitchen.hpp"

using namespace Plazza;

void Kitchen::cook(Pizza &pizza)
{
  auto recipe = _recipes.find(pizza.getType());
  if (recipe == _recipes.end())
    throw std::runtime_error("Unknown pizza type.");
  auto ingredients = recipe->second;
  for (auto ing: ingredients) {
    if (ing == Fridge::Ingredient::COUNT)
      break;
    _fridge.consume(ing);
  }
  pizza.setState(Pizza::State::COOKING);
  // TODO: Assign the pizza to a cook and simulate time
}

Kitchen::Fridge::Fridge()
{
  for (auto &ingredient: _stock)
    ingredient.store(MAX_CAPACITY, std::memory_order_release);
}

auto Kitchen::Fridge::get(Ingredient ing) const -> uint8_t
{
  return _stock[static_cast<std::size_t>(ing)].load(std::memory_order_acquire);
}

void Kitchen::Fridge::consume(Ingredient ing, uint8_t amount)
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

void Kitchen::Fridge::refill(Ingredient ing, uint8_t amount)
{
  _stock[static_cast<std::size_t>(ing)].store(
    amount, std::memory_order_release);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

const Kitchen::RecipeBook Kitchen::_recipes = {
  {Pizza::Margarita,
   [] {
     std::array<Fridge::Ingredient, MAX_INGREDIENTS> arr{};
     arr[0] = Fridge::Ingredient::Dough;
     arr[1] = Fridge::Ingredient::Tomato;
     arr[2] = Fridge::Ingredient::Gruyere;
     for (size_t i = 3; i < MAX_INGREDIENTS; ++i)
       arr[i] = Fridge::Ingredient::COUNT;
     return arr;
   }()},

  {Pizza::Regina,
   [] {
     std::array<Fridge::Ingredient, MAX_INGREDIENTS> arr{};
     arr[0] = Fridge::Ingredient::Dough;
     arr[1] = Fridge::Ingredient::Tomato;
     arr[2] = Fridge::Ingredient::Gruyere;
     arr[3] = Fridge::Ingredient::Ham;
     arr[4] = Fridge::Ingredient::Mushrooms;
     for (size_t i = 5; i < MAX_INGREDIENTS; ++i)
       arr[i] = Fridge::Ingredient::COUNT;
     return arr;
   }()},

  {Pizza::Americana,
   [] {
     std::array<Fridge::Ingredient, MAX_INGREDIENTS> arr{};
     arr[0] = Fridge::Ingredient::Dough;
     arr[1] = Fridge::Ingredient::Tomato;
     arr[2] = Fridge::Ingredient::Gruyere;
     arr[3] = Fridge::Ingredient::Steak;
     for (size_t i = 4; i < MAX_INGREDIENTS; ++i)
       arr[i] = Fridge::Ingredient::COUNT;
     return arr;
   }()},

  {Pizza::Fantasia,
   [] {
     std::array<Fridge::Ingredient, MAX_INGREDIENTS> arr{};
     arr[0] = Fridge::Ingredient::Dough;
     arr[1] = Fridge::Ingredient::Tomato;
     arr[2] = Fridge::Ingredient::Eggplant;
     arr[3] = Fridge::Ingredient::GoatCheese;
     arr[4] = Fridge::Ingredient::ChiefLove;
     for (size_t i = 5; i < MAX_INGREDIENTS; ++i)
       arr[i] = Fridge::Ingredient::COUNT;
     return arr;
   }()},
};
