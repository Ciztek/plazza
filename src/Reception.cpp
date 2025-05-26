#include <iostream>

#include "Reception.hpp"
#include "logging/Logger.hpp"
#include "reception/TicketFacade.hpp"

constexpr auto HELP =
  ("Available commands:\n"
   "  help: Show this help message\n"
   "  quit: Exit the program\n"
   "  status: Show the status of the kitchen\n"
   "  pizza ordering\n");

constexpr auto ASCII_ART = R"(
    ____                                _            _   ____
   / __ \                              | |          | | |___                   \
  | |  | |_   _____ _ __ ___ ___   ___ | | _____  __| |   __) |
  | |  | \ \ / / _ \ '__/ __/ _ \ / _ \| |/ / _ \/ _` |  |__ <
  | |__| |\ V /  __/ | | (_| (_) | (_) |   <  __/ (_| |  ___) |
   \____/  \_/ \___|_|  \___\___/ \___/|_|\_\___|\__,_| |____/
  |  ____|       | |                        | |
  | |__ ___  _ __| | ___ __   ___   ___ __ _| |_   _ _ __  ___  ___
  |  __/ _ \| '__| |/ / '_ \ / _ \ / __/ _` | | | | | '_ \/ __|/ _             \
  | | | (_) | |  |   <| |_) | (_) | (_| (_| | | |_| | |_) \__ \  __/
  |_|  \___/|_|  |_|\_\ .__/ \___/ \___\__,_|_|\__, | .__/|___/\___|
                      | |                       __/ | |
                      |_|                      |___/|_|
 )";

namespace {
  void process_order(std::string &command)
  {
    Log::debug << "processing order: [" + command + "]";
    auto ticket = take_order_ticket(command);

    if (ticket.is_error())
      return;
    for (const auto &order: ticket.value())
      std::cout << "type:" << order.type << "| size: " << order.size;
  }

  auto run_command(std::string &command) -> bool
  {
    if (command == "help") {
      std::cout << HELP;
      return true;
    }
    if (command == "quit")
      return false;
    process_order(command);
    return true;
  }
}  // namespace

void run_reception_repl()
{
  std::string command;
  std::cout << ASCII_ART;
  bool is_running = true;

  while (is_running) {
    std::cout << "> " << std::flush;
    if (!std::getline(std::cin, command))
      is_running = false;
    if (command.empty())
      continue;
    is_running = run_command(command);
  }
}
