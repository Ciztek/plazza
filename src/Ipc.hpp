#ifndef IPC_HPP_
#define IPC_HPP_

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "ErrorOr.hpp"

class Ipc {
public:
  Ipc(int port, const std::string &address = "127.0.0.1");
  ~Ipc() noexcept;

  Ipc(const Ipc &) = delete;
  auto operator=(const Ipc &) -> Ipc & = delete;
  Ipc(Ipc &&) = delete;
  auto operator=(Ipc &&) -> Ipc & = delete;

  auto add_kitchen() -> MaybeError;

  auto
  send_to_kitchen(int kitchen_id, const std::string &message) -> MaybeError;
  auto send_to_all_kitchens(const std::string &message) -> MaybeError;

protected:

private:
  std::vector<int> sockets;
  int reception_socket;
};

#endif /* !IPC_HPP_ */
