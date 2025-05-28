#include "UnixDgramSocket.hpp"
#include "ErrorOr.hpp"
#include "logging/Logger.hpp"

#include <array>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

UnixDgramSocket::UnixDgramSocket() : sock_fd(socket(AF_UNIX, SOCK_DGRAM, 0))
{
  if (sock_fd < 0)
    Log::failed << "cannot initialize socket: " << std::strerror(errno);
}

UnixDgramSocket::~UnixDgramSocket()
{
  cleanup();
}

UnixDgramSocket::UnixDgramSocket(UnixDgramSocket &&other) noexcept
  : sock_fd(other.sock_fd), bound_path(std::move(other.bound_path))
{
  other.sock_fd = -1;
}

auto UnixDgramSocket::
operator=(UnixDgramSocket &&other) noexcept -> UnixDgramSocket &
{
  if (this != &other) {
    cleanup();
    sock_fd = other.sock_fd;
    bound_path = std::move(other.bound_path);
    other.sock_fd = -1;
  }
  return *this;
}

auto UnixDgramSocket::bind(std::string_view path) -> bool
{
  if (sock_fd < 0)
    return false;

  bound_path = path;
  ::unlink(bound_path.c_str());

  auto m_addr = make_sockaddr(path);
  if (m_addr.is_error())
    return false;

  auto addr = m_addr.value();
  if (::bind(sock_fd, reinterpret_cast<sockaddr *>(&addr), sizeof addr) < 0) {
    Log::failed << "bind failed: " << std::strerror(errno);
    return false;
  }

  return true;
}

auto UnixDgramSocket::
  send_to(std::string_view path, std::string_view message) const -> bool
{
  if (sock_fd < 0)
    return false;

  auto m_addr = make_sockaddr(path);
  if (m_addr.is_error())
    return false;

  auto addr = m_addr.value();
  ssize_t sent = sendto(
    sock_fd,
    message.data(),
    message.size(),
    0,
    reinterpret_cast<sockaddr *>(&addr),
    sizeof addr);

  if (sent < 0) {
    Log::failed << "send failed: " << std::strerror(errno);
    return false;
  }

  return true;
}

auto UnixDgramSocket::receive() const
  -> std::optional<std::pair<std::string, std::string>>
{
  if (sock_fd < 0)
    return std::nullopt;

  std::array<char, message_byte_length> buffer{};
  sockaddr_un from{};
  socklen_t from_len = sizeof from;

  ssize_t len = recvfrom(
    sock_fd,
    buffer.data(),
    buffer.size(),
    0,
    reinterpret_cast<sockaddr *>(&from),
    &from_len);

  if (len < 0) {
    Log::failed << "recv failed: " << std::strerror(errno);
    return std::nullopt;
  }

  std::string message(buffer.data(), len);
  std::string from_path(from.sun_path);
  return std::make_pair(std::move(message), std::move(from_path));
}

void UnixDgramSocket::cleanup()
{
  if (sock_fd >= 0) {
    ::close(sock_fd);
    sock_fd = -1;
  }

  if (!bound_path.empty())
    ::unlink(bound_path.c_str());
}

auto UnixDgramSocket::make_sockaddr(std::string_view path)
  -> ErrorOr<sockaddr_un>
{
  sockaddr_un addr{};
  addr.sun_family = AF_UNIX;

  MUST(path.size() <= sizeof addr.sun_path, "Socket path too long");

  std::memcpy(addr.sun_path, path.data(), path.size());
  addr.sun_path[path.size()] = '\0';

  return addr;
}
