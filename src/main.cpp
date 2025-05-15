#include <cstdlib>

#include "Config.hpp"
#include "ErrorOr.hpp"
#include "Repl.hpp"

namespace {

  auto wrappedMain(int argc, char *argv[]) -> MaybeError
  {
    Plazza::Repl repl;

    TRY(CONFIG_FILE.init());
    TRY(CONFIG_ARGS.init(argc, argv));
    TRY(repl.run());
    return Nil{};
  }
}  // namespace

auto main(int argc, char *argv[]) -> int
{
  if (wrappedMain(argc, argv).is_error())
    return EXIT_TEK;
}
