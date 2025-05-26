#pragma once

#include <chrono>
#include <cstdint>

#include "ErrorOr.hpp"

namespace clk = std::chrono;

struct Params {
  double multiplier;
  std::uint8_t cook;
  clk::milliseconds time;

  static auto parse_arguments(int argc, std::span<char *>) -> ErrorOr<Params>;
};

#include <iostream>

inline auto operator<<(std::ostream &os, const Params &p) -> std::ostream &
{
  os << "Params{ multiplier=" << p.multiplier << ", cook="
     << static_cast<int>(p.cook) << ", time=" << p.time.count() << "ms }";
  return os;
}
