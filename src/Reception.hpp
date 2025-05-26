#pragma once

#include "Pizza.hpp"

struct Order {
  size_t type;
  Pizza::Size size;
};

void run_reception_repl();
