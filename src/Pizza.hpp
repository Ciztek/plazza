#pragma once

#include <atomic>
#include <cstdint>
#include <iostream>

class Pizza {
public:
  enum Size : uint8_t {
    S = 1,
    M = 2,
    L = 3,
    XL = 4,
    XXL = 5
  };

  enum State : uint8_t {
    UNUSED = 0,
    ASSIGNED = 1,
    COOKING = 2,
    READY = 3
  };

  Pizza() = default;
  explicit Pizza(uint8_t type, Size size);
  ~Pizza() = default;

  auto set(uint8_t type, Size size, State state) -> Pizza &;
  auto setState(State newState) -> Pizza &;
  auto clear() -> Pizza &;

  [[nodiscard]] auto raw() const -> uint16_t;
  [[nodiscard]] auto getType() const -> uint8_t;
  [[nodiscard]] auto getSize() const -> Size;
  [[nodiscard]] auto getState() const -> State;

private:
  std::atomic<uint16_t> data = 0;
};

#define OS_SEND_STRINGIFIED_ENUM(key) case Pizza::key: os << (#key); break

inline auto operator<<(std::ostream &os, const Pizza &pizza) -> std::ostream &
{
  os << "Pizza(type: " << unsigned(pizza.getType());

  os << ", size: ";
  switch (pizza.getSize()) {
    OS_SEND_STRINGIFIED_ENUM(S);
    OS_SEND_STRINGIFIED_ENUM(M);
    OS_SEND_STRINGIFIED_ENUM(L);
    OS_SEND_STRINGIFIED_ENUM(XL);
    OS_SEND_STRINGIFIED_ENUM(XXL);
  }

  os << ", state: ";
  switch (pizza.getState()) {
    OS_SEND_STRINGIFIED_ENUM(UNUSED);
    OS_SEND_STRINGIFIED_ENUM(ASSIGNED);
    OS_SEND_STRINGIFIED_ENUM(COOKING);
    OS_SEND_STRINGIFIED_ENUM(READY);
  }

  os << ")";
  return os;
}
