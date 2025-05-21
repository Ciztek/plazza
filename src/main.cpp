#include <cstdlib>
#include <span>

#include "Config.hpp"
#include "ErrorOr.hpp"

namespace {

#define IS_ONLY(str, valid) (strspn(str, valid) == strlen(str))

  auto validateIntArgs(char *arg) -> MaybeError
  {
    if (!IS_ONLY(arg, "0123456789"))
      return Error("Time parameter is not a positive intger");
    if (std::stoul(arg, nullptr, 10) == 0)
      return Error("Time parameter cannot be 0");
    return Nil{};
  }

  auto validateDoubleArgs(char *arg) -> MaybeError
  {
    if (IS_ONLY(arg, "0123456789."))
      return Error("Multiplier parameter is not a double");
    try {
      std::stod(arg, nullptr);
    } catch (...) {
      return Error("Multiplier parameter is not a double");
    }
    return Nil{};
  }

#undef IS_ONLY

  auto
  parse_params(int argc, std::span<char *> args) -> ErrorOr<Config::Params>
  {
    Config::Params params;
    if (argc != 4)
      return Error("Not enough arguments");

    TRY(validateDoubleArgs(args[1]));
    TRY(validateIntArgs(args[2]));
    TRY(validateIntArgs(args[3]));

    params.multiplier = std::stod(args[1], nullptr);
    params.cook = std::stoul(args[2], nullptr, 10);
    params.time = std::chrono::milliseconds(std::stoul(args[3], nullptr, 10));
    return params;
  }
}  // namespace

namespace {

  auto wrappedMain(int argc, std::span<char *> args) -> MaybeError
  {
    TRY(CONFIG_FILE.init());
    auto params = TRY(parse_params(argc, args));
    (void)params;
    return Nil{};
  }
}  // namespace

auto main(int argc, char *argv[]) -> int
{
  std::span<char *> args(argv, argv + argc);
  if (wrappedMain(argc, args).is_error())
    return EXIT_TEK;
}
