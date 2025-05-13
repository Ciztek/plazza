#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "Pizza.hpp"

namespace Plazza {

  class Kitchen {
  public:
    Kitchen() = default;

    Kitchen(const Kitchen &) = delete;
    Kitchen(Kitchen &&) = delete;
    auto operator=(const Kitchen &) -> Kitchen & = delete;
    auto operator=(Kitchen &&) -> Kitchen & = delete;

    void cook(Pizza &pizza);

    ~Kitchen() = default;

  private:
    class Fridge {
    public:
      Fridge();
      ~Fridge() = default;

      [[nodiscard]] auto get(size_t ing) const -> uint8_t;
      void consume(size_t ing, uint8_t amount = 1);
      void refill(size_t ing, uint8_t amount = MAX_CAPACITY);

      static constexpr uint8_t MAX_CAPACITY = 5;

    private:
      std::vector<std::atomic<uint8_t>> _stock;
    };

    Fridge _fridge;
  };
}  // namespace Plazza
