#include <cstdlib>

#include "ErrorOr.hpp"
#include "Repl.hpp"

namespace {

  auto wrappedMain() -> MaybeError
  {
    Plazza::Repl repl;

    TRY(repl.run());
    return Nil{};
  }
}  // namespace

auto main() -> int
{
  if (wrappedMain().is_error())
    return 84;
  return EXIT_SUCCESS;
}
