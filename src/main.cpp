#include <cstdlib>

#include "ErrorOr.hpp"
#include "Plazza.hpp"

namespace {

  auto wrappedMain(int, char *[]) -> MaybeError
  {
    TRY(Config::init());
    return Nil{};
  }
}  // namespace

auto main(int argc, char *argv[]) -> int
{
  if (wrappedMain(argc, argv).is_error())
    return EXIT_TEK;
}
