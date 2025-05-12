#pragma once

#include <filesystem>

namespace Config {
  void parse(const std::filesystem::path &path);
}  // namespace Config
