#include <cstdlib>
#include <span>

#include "Config.hpp"
#include "ErrorOr.hpp"

namespace {

  auto wrappedMain(int argc, std::span<char *> argv) -> MaybeError
  {
    auto conf = TRY(Config::Init::fileConf());
    auto params = TRY(Config::Init::argConf(argc, argv));
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
