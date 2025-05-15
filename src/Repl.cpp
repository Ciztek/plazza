
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "Config.hpp"
#include "ErrorOr.hpp"
#include "Pizza.hpp"
#include "Repl.hpp"

constexpr auto HELP =
  ("Available commands:\n"
   "  help: Show this help message\n"
   "  quit: Exit the program\n"
   "  status: Show the status of the kitchen\n"
   "  pizza ordering\n");

constexpr auto ASCII_ART =
  R"(    ____                                _            _   ____
   / __ \                              | |          | | |___ \
  | |  | |_   _____ _ __ ___ ___   ___ | | _____  __| |   __) |
  | |  | \ \ / / _ \ '__/ __/ _ \ / _ \| |/ / _ \/ _` |  |__ <
  | |__| |\ V /  __/ | | (_| (_) | (_) |   <  __/ (_| |  ___) |
   \____/  \_/ \___|_|  \___\___/ \___/|_|\_\___|\__,_| |____/
  |  ____|       | |                        | |
  | |__ ___  _ __| | ___ __   ___   ___ __ _| |_   _ _ __  ___  ___
  |  __/ _ \| '__| |/ / '_ \ / _ \ / __/ _` | | | | | '_ \/ __|/ _ \
  | | | (_) | |  |   <| |_) | (_) | (_| (_| | | |_| | |_) \__ \  __/
  |_|  \___/|_|  |_|\_\ .__/ \___/ \___\__,_|_|\__, | .__/|___/\___|
                      | |                       __/ | |
                      |_|                      |___/|_|
 )";

using namespace Plazza;

auto Repl::help() -> MaybeError
{
  (void)_command;
  std::cout << HELP;
  return Nil{};
}

auto Repl::quit() -> MaybeError
{
  Repl::running = false;
  return Nil{};
}

auto Repl::status() -> MaybeError
{
  (void)_command;
  std::cout << "Kitchen status:\n";
  return Nil{};
}

struct Order {
  size_t type;
  Pizza::Size size;
};

namespace {
  auto
  split(const std::string &str, char delimiter) -> std::vector<std::string>
  {
    std::vector<std::string> tokens;
    size_t start = 0;
    while (start < str.size()) {
      size_t pos = str.find(delimiter, start);
      auto token = str.substr(start, pos - start);
      if (!token.empty())
        tokens.push_back(token);
      if (pos == std::string::npos)
        break;
      start = pos + 1;
    }
    return tokens;
  }

  constexpr auto AVAILABLE_SIZES = std::
    array<std::string, 5>{"S", "M", "L", "XL", "XXL"};
  constexpr auto SIZE_TRANSLATES = std::array<Pizza::Size, 5>{
    Pizza::Size::S,
    Pizza::Size::M,
    Pizza::Size::L,
    Pizza::Size::XL,
    Pizza::Size::XXL};
}  // namespace

auto Repl::order() -> MaybeError
{
  std::vector<Order> orders;
  size_t quantity = 0;
#define ISALPHA(str)                                                           \
    std::all_of((str).begin(), (str).end(), [](unsigned char c) { return std::isalpha(c); })

  for (const auto &order: split(_command, ';')) {
    auto tokens = split(order, ' ');
    if (tokens.size() != 3)
      return Error("Invalid order format");
    Order newOrder;
    if (!ISALPHA(tokens[0]))
      return Error("Invalid pizza type");

    if (std::ranges::find(AVAILABLE_SIZES, tokens[1]) == AVAILABLE_SIZES.end())
      return Error("Invalid pizza size");
    if (tokens[2].size() < 2 || tokens[2][0] != 'x' || tokens[2][1] == '0')
      return Error("Invalid pizza quantity");
    if (!std::all_of(tokens[2].begin() + 1, tokens[2].end(), ::isdigit))
      return Error("Invalid pizza quantity");

    newOrder.type = TRY(CONFIG_FILE.getRecipesIds().lookup(tokens[0]));
    newOrder.size = SIZE_TRANSLATES[std::distance(
      AVAILABLE_SIZES.begin(), std::ranges::find(AVAILABLE_SIZES, tokens[1]))];
    quantity = std::stoul(tokens[2].substr(1), nullptr, 10);
    for (size_t i = 0; i < quantity; ++i) {
      orders.push_back(newOrder);
      std::cout
        << "Order: " << CONFIG_FILE.getRecipesIds().lookup(newOrder.type)
        << " " << newOrder.size << "\n";
    }
  }
  return Nil{};
}

auto Repl::runCommand() -> MaybeError
{
  if (_command == "help")
    return help();
  if (_command == "quit")
    return quit();
  if (_command == "status")
    return status();
  auto res = order();
  if (res.has_value())
    return Nil{};
  std::cout << "Unknown command: " << _command << "\n";
  return Nil{};
}

auto Repl::run() -> MaybeError
{
  Repl::running = true;

  std::cout << ASCII_ART;

  while (Repl::running) {
    std::cout << "> ";
    if (!std::getline(std::cin, _command))
      Repl::running = false;
    if (_command.empty())
      continue;
    MUST(runCommand(), "Not supposed to be even possible");
  }
  return Nil{};
}
