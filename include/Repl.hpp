#pragma once

#include <map>
#include <string>

#include "ErrorOr.hpp"

namespace Plazza {
  class Repl {
  public:
    Repl() = default;
    ~Repl() = default;

    auto runCommand() -> MaybeError;

    auto help() -> MaybeError;
    auto quit() -> MaybeError;
    auto order() -> MaybeError;
    auto status() -> MaybeError;

    auto run() -> MaybeError;

  private:
    bool running;
    static const std::map<std::string, auto (Repl::*)()->MaybeError> COMMANDS;
    std::string _command;
  };
}  // namespace Plazza
