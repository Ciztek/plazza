#include "ArgParser.hpp"

#include <algorithm>
#include <charconv>
#include <string_view>

namespace {
  template <typename> inline constexpr bool always_false = false;

  template <typename T> constexpr auto type_name() -> std::string
  {
    if constexpr (std::is_same_v<T, int>)
      return "integer";
    if constexpr (std::is_same_v<T, double>)
      return "double";
    return "unsupported type";
  }

  template <typename T>
  auto parse_positive(std::string_view arg, const std::string &param_name)
    -> ErrorOr<T>
  {
    if constexpr (!(std::is_same_v<T, int> || std::is_same_v<T, double>))
      static_assert(always_false<T>, "Unsupported type");

    T value{};
    auto [ptr, ec] = std::
      from_chars(arg.data(), arg.data() + arg.size(), value);
    if (ec != std::errc{} || ptr != arg.data() + arg.size() || value <= 0)
      return Error(param_name + " must be a valid positive " + type_name<T>());

    return value;
  }
}  // namespace

auto Params::parse_arguments(int argc, std::span<char *> argv)
  -> ErrorOr<Params>
{
  if (argc != 3)
    return Error("Expected exactly 3 arguments");

  auto ctm = TRY(parse_positive<double>(argv[0], "cooking time multiplier"));
  auto cpk = TRY(parse_positive<int>(argv[1], "cook per kitchen"));
  auto ttr = TRY(parse_positive<int>(argv[2], "time to restock"));

  Params params(
    ctm,
    static_cast<std::uint8_t>(std::clamp(cpk, 1, 255)),
    clk::milliseconds(ttr));

  return params;
}
