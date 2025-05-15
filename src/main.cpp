#include <cstdlib>

#include "Config.hpp"
#include "ErrorOr.hpp"

namespace {

  auto wrappedMain(int argc, char *argv[]) -> MaybeError
  {
    TRY(CONFIG_FILE.init());
    TRY(CONFIG_ARGS.init(argc, argv));
    return Nil{};
  }
}  // namespace

auto main(int argc, char *argv[]) -> int
{
  if (wrappedMain(argc, argv).is_error())
    return EXIT_TEK;
}
