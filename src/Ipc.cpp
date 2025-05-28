#include "Ipc.hpp"

Ipc::Ipc(int port, const std::string &address) : reception_socket(-1)
{
  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  reception_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (reception_socket < 0)
    throw std::runtime_error("Failed to create socket");
  int opt = 1;
  if (setsockopt(reception_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))
      < 0) {
    close(reception_socket);
    throw std::runtime_error("Failed to set socket options");
  }
  if (inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr) <= 0) {
    close(reception_socket);
    throw std::runtime_error("Invalid address");
  }
  if (
    bind(
      reception_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))
    < 0) {
    close(reception_socket);
    throw std::runtime_error("Failed to bind socket");
  }
  if (listen(reception_socket, SOMAXCONN) < 0) {
    close(reception_socket);
    throw std::runtime_error("Failed to listen on socket");
  }
}

Ipc::~Ipc() noexcept
{
  for (int sock: sockets)
    close(sock);
  if (reception_socket != -1)
    close(reception_socket);
}

auto Ipc::add_kitchen() -> MaybeError
{
  sockaddr_in client_addr{};
  socklen_t addr_len = sizeof(client_addr);

  int client_socket = accept(
    reception_socket, (struct sockaddr *)&client_addr, &addr_len);
  if (client_socket < 0)
    return Error("Failed to accept connection");
  sockets.push_back(client_socket);
  return {};
}

auto Ipc::send_to_kitchen(int kitchen_id, const std::string &message)
  -> MaybeError
{
  if (kitchen_id < 0 || kitchen_id >= static_cast<int>(sockets.size()))
    return Error("Invalid kitchen ID");

  int sock = sockets[kitchen_id];
  ssize_t sent_bytes = send(sock, message.c_str(), message.size(), 0);
  if (sent_bytes < 0)
    return Error("Failed to send message to kitchen");

  return {};
}

auto Ipc::send_to_all_kitchens(const std::string &message) -> MaybeError
{
  for (int sock: sockets) {
    ssize_t sent_bytes = send(sock, message.c_str(), message.size(), 0);
    if (sent_bytes < 0)
      return Error("Failed to send message to one or more kitchens");
  }
  return {};
}
