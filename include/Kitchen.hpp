#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "Config.hpp"
#include "ErrorOr.hpp"
#include "Pizza.hpp"

namespace Plazza {

  class Kitchen {
  public:
    Kitchen(const Config::FileConfig &conf, const Config::Params &params);

    Kitchen(const Kitchen &) = delete;
    Kitchen(Kitchen &&) = delete;
    auto operator=(const Kitchen &) -> Kitchen & = delete;
    auto operator=(Kitchen &&) -> Kitchen & = delete;

    auto cook(Pizza &pizza) -> MaybeError;

    ~Kitchen() = default;

  private:
    class Fridge {
    public:
      Fridge(const Config::FileConfig &conf);
      ~Fridge() = default;

      [[nodiscard]] auto get(size_t ing) const -> uint8_t;
      auto consume(size_t ing, uint8_t amount = 1) -> MaybeError;
      auto refill(size_t ing, uint8_t amount = MAX_CAPACITY) -> MaybeError;

      static constexpr uint8_t MAX_CAPACITY = 5;

    private:
      std::vector<std::atomic<uint8_t>> _stock;
    };

    const Config::FileConfig &_conf;
    const Config::Params _params;
    Fridge _fridge{_conf};
  };
}  // namespace Plazza
