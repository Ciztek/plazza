#pragma once

#include <atomic>
#include <cstdint>

#define FIELD_X_BYTES(x) ((1u << (x)) - 1u)

namespace Plazza {

  class Pizza {
  public:
    enum Type : uint8_t {
      Regina = 1,
      Margarita = 2,
      Americana = 3,
      Fantasia = 4
    };

    enum Size : uint8_t {
      S = 1,
      M = 2,
      L = 3,
      XL = 4,
      XXL = 5
    };

    enum class State : uint8_t {
      UNUSED,
      ASSIGNED,
      COOKING,
      READY
    };

    Pizza() = default;
    explicit Pizza(uint8_t type, uint8_t size, State state);
    ~Pizza() = default;

    auto set(uint8_t type, uint8_t size, State state) -> Pizza &;
    auto setState(State newState) -> Pizza &;
    auto clear() -> Pizza &;

    [[nodiscard]] auto raw() const -> uint8_t;
    [[nodiscard]] auto getType() const -> Type;
    [[nodiscard]] auto getSize() const -> Size;
    [[nodiscard]] auto getState() const -> State;

  private:
    std::atomic<uint8_t> data = 0;

    static constexpr uint8_t TYPE_SHIFT = 5;
    static constexpr uint8_t SIZE_SHIFT = 2;

    static constexpr uint8_t MASK3 = FIELD_X_BYTES(3);

    static constexpr uint8_t TYPE_MASK = MASK3 << TYPE_SHIFT;
    static constexpr uint8_t SIZE_MASK = MASK3 << SIZE_SHIFT;
    static constexpr uint8_t STATE_MASK = FIELD_X_BYTES(2);
  };
}  // namespace Plazza
