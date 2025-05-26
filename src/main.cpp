#include <cstdlib>
#include <span>

#include "ArgParser.hpp"
#include "KitchenCatalog.hpp"

#include "ErrorOr.hpp"

namespace {
  constexpr int EXIT_TEK = 84;

  auto wrappedMain(int argc, std::span<char *> argv) -> MaybeError
  {
    auto params = TRY(Params::parse_arguments(argc, argv));

    Log::info << params;
    auto catalog = TRY(KitchenCalatog::load_from_file("catalog.json"));
    return Nil{};
  }

}  // namespace

auto main(int argc, char *argv[]) -> int
{
  argc--;
  argv++;
  std::span<char *> args(argv, argv + argc);
  if (wrappedMain(argc, args).is_error())
    return EXIT_TEK;
}
