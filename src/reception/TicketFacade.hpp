#pragma once

#include <string>
#include <vector>

#include "TicketInternal.hpp"

struct PizzaOrder {
  std::string type;
  std::string size;
  int quantity;
};

auto take_order_ticket(const std::string &str) -> ErrorOr<std::vector<Order>>;
