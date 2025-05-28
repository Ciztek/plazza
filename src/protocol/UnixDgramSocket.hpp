#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <sys/un.h>
#include <unistd.h>
#include "ErrorOr.hpp"

class UnixDgramSocket {
public:
  /* Lets start with a simple 4-bit type + data */
  static constexpr size_t message_byte_length = 8;

  UnixDgramSocket();
  ~UnixDgramSocket();

  UnixDgramSocket(const UnixDgramSocket &) = delete;
  auto operator=(const UnixDgramSocket &) -> UnixDgramSocket & = delete;

  UnixDgramSocket(UnixDgramSocket &&other) noexcept;
  auto operator=(UnixDgramSocket &&other) noexcept -> UnixDgramSocket &;

  auto bind(std::string_view path) -> bool;
  [[nodiscard]] auto
  send_to(std::string_view path, std::string_view message) const -> bool;
  [[nodiscard]] auto
  receive() const -> std::optional<std::pair<std::string, std::string>>;

private:
  int sock_fd;
  std::filesystem::path bound_path;

  static auto make_sockaddr(std::string_view path) -> ErrorOr<sockaddr_un>;
  void cleanup();
};
