#include "Pizza.hpp"
#include <array>
#include <cstdint>

using namespace Plazza;

namespace {
  constexpr auto FIELD_MASK(uint8_t bits) -> uint16_t
  {
    return (1U << bits) - 1U;
  }

  enum BitsOrder : std::uint8_t {
    STAT_ORDER,
    LEFT_ORDER,
    SIZE_ORDER,
    TYPE_ORDER,
    ORDER_COUNT
  };

  enum BitsSize : std::uint8_t {
    STAT_BITS = 2,
    LEFT_BITS = 3,
    SIZE_BITS = 3,
    TYPE_BITS = 8,
  };

  constexpr std::array<uint8_t, ORDER_COUNT> BitsSizes =
    {STAT_BITS, LEFT_BITS, SIZE_BITS, TYPE_BITS};

  constexpr auto computeShift(BitsOrder order) -> uint8_t
  {
    uint8_t shift = 0;
    for (size_t i = 0; i < order; ++i)
      shift += BitsSizes[i];
    return shift;
  }

  constexpr uint8_t STAT_SHIFT = computeShift(STAT_ORDER);
  constexpr uint8_t LEFT_SHIFT = computeShift(LEFT_ORDER);
  constexpr uint8_t SIZE_SHIFT = computeShift(SIZE_ORDER);
  constexpr uint8_t TYPE_SHIFT = computeShift(TYPE_ORDER);

  enum Mask : std::uint16_t {
    STAT_MASK = FIELD_MASK(STAT_BITS) << STAT_SHIFT,
    LEFT_MASK = FIELD_MASK(LEFT_BITS) << LEFT_SHIFT,
    SIZE_MASK = FIELD_MASK(SIZE_BITS) << SIZE_SHIFT,
    TYPE_MASK = FIELD_MASK(TYPE_BITS) << TYPE_SHIFT
  };
}  // anonymous namespace

#define ENCODE_FIELD(value, FIELD)                                             \
  (((value) & FIELD_MASK(FIELD##_BITS)) << FIELD##_SHIFT)
#define DECODE_FIELD(value, FIELD)                                             \
  (((value) & FIELD##_MASK) >> FIELD##_SHIFT)

Pizza::Pizza(uint8_t type, Size size, State state)
{
  set(type, size, state);
}

auto Pizza::set(uint8_t type, Size size, State state) -> Pizza &
{
  uint16_t value = ENCODE_FIELD(type, TYPE) | ENCODE_FIELD(size, SIZE)
    | ENCODE_FIELD(0, LEFT) | ENCODE_FIELD(state, STAT);

  data.store(value, std::memory_order_release);
  return *this;
}

auto Pizza::setState(State newState) -> Pizza &
{
  uint16_t current = raw();
  uint16_t updated = (current & ~STAT_MASK)
    | ENCODE_FIELD(static_cast<uint8_t>(newState), STAT);

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
  return static_cast<uint8_t>(DECODE_FIELD(raw(), TYPE));
}

auto Pizza::getSize() const -> Pizza::Size
{
  return static_cast<Pizza::Size>(DECODE_FIELD(raw(), SIZE));
}

auto Pizza::getState() const -> Pizza::State
{
  return static_cast<State>(DECODE_FIELD(raw(), STAT));
}
