#include <cstdlib>
#include <span>

#include "ErrorOr.hpp"

#include "ArgParser.hpp"
#include "Kitchen.hpp"
#include "KitchenCatalog.hpp"
#include "Repl.hpp"

namespace {
  constexpr int EXIT_TEK = 84;

  auto wrappedMain(int argc, std::span<char *> argv) -> MaybeError
  {
    auto params = TRY(Params::parse_arguments(argc, argv));
    Log::info << params;

    auto catalog = TRY(KitchenCalatog::load_from_file("catalog.json"));
    Log::info << "Loaded with " << catalog.recipes.size() << " recipes!";

    Kitchen test(catalog, params.cook);
    Pizza p(TRY(catalog.recipes.at_value("margarita")), Pizza::XL);

    Log::info << p;

    Plazza::Repl repl;
    TRY(repl.run());
    return {};
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
