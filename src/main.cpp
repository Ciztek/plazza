#include <cstdlib>
#include <span>

#include "Config.hpp"
#include "ErrorOr.hpp"

namespace {

  auto wrappedMain(int argc, std::span<char *> args) -> MaybeError
  {
    TRY(CONFIG_FILE.init());
    TRY(CONFIG_ARGS.init(argc, args));
    return Nil{};
  }
}  // namespace

auto main(int argc, char *argv[]) -> int
{
  std::span<char *> args(argv, argv + argc);
  if (wrappedMain(argc, args).is_error())
    return EXIT_TEK;
}
