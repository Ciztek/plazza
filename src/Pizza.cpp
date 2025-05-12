#include "Pizza.hpp"

using namespace Plazza;

Pizza::Pizza(uint8_t type, uint8_t size, State state)
{
  set(type, size, state);
}

auto Pizza::set(uint8_t type, uint8_t size, State state) -> Pizza &
{
  uint8_t value = ((type & MASK3) << TYPE_SHIFT)
    | ((size & MASK3) << SIZE_SHIFT)
    | (static_cast<uint8_t>(state) & STATE_MASK);
  data.store(value, std::memory_order_release);
  return *this;
}

auto Pizza::setState(State newState) -> Pizza &
{
  uint8_t current = raw();
  uint8_t updated = (current & ~STATE_MASK)
    | (static_cast<uint8_t>(newState) & STATE_MASK);
  data.store(updated, std::memory_order_release);
  return *this;
}

auto Pizza::clear() -> Pizza &
{
  data.store(0, std::memory_order_release);
  return *this;
}

auto Pizza::raw() const -> uint8_t
{
  return data.load(std::memory_order_acquire);
}

auto Pizza::getType() const -> uint8_t
{
  return (raw() & TYPE_MASK) >> TYPE_SHIFT;
}

auto Pizza::getSize() const -> uint8_t
{
  return (raw() & SIZE_MASK) >> SIZE_SHIFT;
}

auto Pizza::getState() const -> Pizza::State
{
  return static_cast<State>(raw() & STATE_MASK);
}
