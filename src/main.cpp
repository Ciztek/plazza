#include <cstdlib>

#include "Config.hpp"
#include "ErrorOr.hpp"

namespace {

  auto wrappedMain(int, char *[]) -> MaybeError
  {
    TRY(CONFIG_FILE.init());
    return Nil{};
  }
}  // namespace

auto main(int argc, char *argv[]) -> int
{
  if (wrappedMain(argc, argv).is_error())
    return EXIT_TEK;
}
