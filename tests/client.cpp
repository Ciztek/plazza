#include "logging/Logger.hpp"
#include "protocol/UnixDgramSocket.hpp"

#include <string>

auto main(int argc, char *argv[]) -> int
{
  if (argc != 2) {
    Log::failed << "Usage: ./client <NUM>";
    return 1;
  }

  int num = std::stoi(argv[1]);
  const std::string server_path = "/tmp/plazza_server";
  const std::string client_path = "/tmp/plazza_client_" + std::to_string(num);

  UnixDgramSocket client;
  if (!client.bind(client_path)) {
    Log::failed << "Client " << num << " failed to bind";
    return 1;
  }

  std::string hello_msg = "hello " + std::to_string(num);
  if (!client.send_to(server_path, hello_msg)) {
    Log::failed << "fail to send";
    return EXIT_FAILURE;
  }

  Log::info << "Client " << num << " sent: " << hello_msg;

  auto result = client.receive();
  if (result) {
    const auto &[msg, from] = *result;
    Log::info << "Client " << num << " received: " << msg << " from " << from;
    if (msg == "done")
      Log::info << "Client " << num << " exiting.";
  }
}
