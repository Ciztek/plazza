#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <thread>
#include <vector>

#include "ErrorOr.hpp"
#include "KitchenCatalog.hpp"
#include "Pizza.hpp"

class Kitchen {
public:
  Kitchen(const KitchenCalatog &catalog, size_t ncook, int id, int port);

  auto cook(Pizza &pizza) -> MaybeError;

  ~Kitchen() = default;

private:
  const KitchenCalatog _catalog;
  std::vector<uint8_t> _fridge;
  size_t _ncook;
  int _port;
  // TODO: find a fix for this
  // std::vector<std::thread> _cooks;
};
