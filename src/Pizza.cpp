
#include <array>
#include <cstdint>
#include <cstring>

#include "Pizza.hpp"

namespace {
  constexpr auto FIELD_MASK(uint8_t bits) -> uint16_t
  {
    return (1U << bits) - 1U;
  }

  enum Field : uint8_t {
    STAT = 0,
    SIZE,
    LEFT,
    TYPE,
  };

  struct FieldEntry {
    Field name;
    uint8_t bits;
    uint8_t shift;
  };

  constexpr auto Fields = []() {
    constexpr std::array<std::pair<Field, uint8_t>, 4> fieldsInfo = {{
      {STAT, 2},
      {SIZE, 3},
      {LEFT, 3},
      {TYPE, 8},
    }};

    std::array<FieldEntry, fieldsInfo.size()> fields{};
    uint8_t shift = 0;
    for (size_t i = 0; i < fieldsInfo.size(); ++i) {
      fields[i] = {
        .name = fieldsInfo[i].first,
        .bits = fieldsInfo[i].second,
        .shift = shift};
      shift += fieldsInfo[i].second;
    }
    return fields;
  }();

  constexpr auto findField(Field name) -> FieldEntry
  {
    for (auto Field: Fields)
      if (Field.name == name)
        return Field;
    throw "Unknown field name";
  }

  constexpr auto getMask(Field name) -> uint16_t
  {
    auto field = findField(name);
    return FIELD_MASK(field.bits) << field.shift;
  }

  constexpr auto encodeField(uint16_t value, Field name) -> uint16_t
  {
    auto field = findField(name);
    return (value & FIELD_MASK(field.bits)) << field.shift;
  }

  constexpr auto decodeField(uint16_t value, Field name) -> uint16_t
  {
    auto field = findField(name);
    return (value >> field.shift) & FIELD_MASK(field.bits);
  }
}  // namespace

Pizza::Pizza(uint8_t type, Size size)
{
  set(type, size, UNUSED);
}

auto Pizza::set(uint8_t type, Size size, State state) -> Pizza &
{
  uint16_t value = encodeField(type, TYPE) | encodeField(size, SIZE)
    | encodeField(0, LEFT) | encodeField(state, STAT);

  data.store(value, std::memory_order_release);
  return *this;
}

auto Pizza::setState(State newState) -> Pizza &
{
  uint16_t current = raw();
  uint16_t updated = (current & ~getMask(STAT)) | encodeField(newState, STAT);

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
  return static_cast<uint8_t>(decodeField(raw(), TYPE));
}

auto Pizza::getSize() const -> Pizza::Size
{
  return static_cast<Pizza::Size>(decodeField(raw(), SIZE));
}

auto Pizza::getState() const -> Pizza::State
{
  return static_cast<State>(decodeField(raw(), STAT));
}
