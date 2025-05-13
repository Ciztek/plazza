#include "Pizza.hpp"

using namespace Plazza;

namespace {
  constexpr auto FIELD_MASK(uint8_t bits) -> uint16_t
  {
    return (1U << bits) - 1U;
  }

  using BitsOrder = enum : std::uint8_t {
    TYPE_ORDER = 0,
    SIZE_ORDER = 1,
    LEFT_ORDER = 2,
    STAT_ORDER = 3,
  };

  using BitsSize = enum : std::uint8_t {
    STAT_BITS = 2,
    LEFT_BITS = 3,
    SIZE_BITS = 3,
    TYPE_BITS = 8,
  };

  using Shift = enum : std::uint8_t {
    STAT_SHIFT = 0,
    LEFT_SHIFT = 2,
    SIZE_SHIFT = 5,
    TYPE_SHIFT = 8
  };

  using Mask = enum : std::uint16_t {
    STAT_MASK = FIELD_MASK(STAT_BITS) << STAT_SHIFT,
    LEFT_MASK = FIELD_MASK(LEFT_BITS) << LEFT_SHIFT,
    SIZE_MASK = FIELD_MASK(SIZE_BITS) << SIZE_SHIFT,
    TYPE_MASK = FIELD_MASK(TYPE_BITS) << TYPE_SHIFT
  };

}  // anonymous namespace

Pizza::Pizza(uint8_t type, Size size, State state)
{
  set(type, size, state);
}

auto Pizza::set(uint8_t type, Size size, State state) -> Pizza &
{
  uint16_t value = ((type & FIELD_MASK(TYPE_BITS)) << TYPE_SHIFT)
    | ((static_cast<uint8_t>(size) & FIELD_MASK(SIZE_BITS)) << SIZE_SHIFT)
    | ((0 & FIELD_MASK(LEFT_BITS)) << LEFT_SHIFT)
    | ((static_cast<uint8_t>(state) & FIELD_MASK(STAT_BITS)) << STAT_SHIFT);
  data.store(value, std::memory_order_release);
  return *this;
}

auto Pizza::setState(State newState) -> Pizza &
{
  uint16_t current = raw();
  uint16_t updated = (current & ~STAT_MASK)
    | (static_cast<uint8_t>(newState) << STAT_SHIFT);
  data.store(updated, std::memory_order_release);
  return *this;
}

auto Pizza::clear() -> Pizza &
{
  data.store(0, std::memory_order_release);
  return *this;
}

auto Pizza::raw() const -> uint16_t
{
  return data.load(std::memory_order_acquire);
}

auto Pizza::getType() const -> uint8_t
{
  return static_cast<uint8_t>((raw() & TYPE_MASK) >> TYPE_SHIFT);
}

auto Pizza::getSize() const -> Pizza::Size
{
  return static_cast<Pizza::Size>((raw() & SIZE_MASK) >> SIZE_SHIFT);
}

auto Pizza::getState() const -> Pizza::State
{
  return static_cast<State>((raw() & STAT_MASK) >> STAT_SHIFT);
}
