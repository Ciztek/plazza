#include <cstddef>

#include "ErrorOr.hpp"
#include "Pizza.hpp"
#include "Reception.hpp"

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

void process_order(std::string &command)
{
  Log::debug << "processing order: [" + command + "]";
}

static auto run_command(std::string &command) -> bool
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

void run_reception_repl()
{
  std::string command;
  std::cout << ASCII_ART;
  bool is_running = true;

  while (is_running) {
    std::cout << "> ";
    if (!std::getline(std::cin, command))
      is_running = false;
    if (command.empty())
      continue;
    is_running = run_command(command);
  }
}
