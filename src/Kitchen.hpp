#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "ErrorOr.hpp"
#include "KitchenCatalog.hpp"
#include "Pizza.hpp"

class Kitchen {
public:
  Kitchen(const KitchenCalatog &catalog, size_t ncook);

  Kitchen(const Kitchen &) = delete;
  Kitchen(Kitchen &&) = delete;
  auto operator=(const Kitchen &) -> Kitchen & = delete;
  auto operator=(Kitchen &&) -> Kitchen & = delete;

  auto cook(Pizza &pizza) -> MaybeError;

  ~Kitchen() = default;

private:
  const KitchenCalatog _catalog;
  std::vector<uint8_t> _fridge;
  size_t _ncook;
};
