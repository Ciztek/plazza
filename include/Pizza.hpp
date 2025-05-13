#pragma once

#include <atomic>
#include <cstdint>

namespace Plazza {

  class Pizza {
  public:
    enum Size : uint8_t {
      S = 1,
      M = 2,
      L = 3,
      XL = 4,
      XXL = 5
    };

    enum class State : uint8_t {
      UNUSED = 0,
      ASSIGNED = 1,
      COOKING = 2,
      READY = 3
    };

    Pizza() = default;
    explicit Pizza(uint8_t type, Size size, State state);
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
}  // namespace Plazza
