#include "logging/Logger.hpp"
#include "protocol/UnixDgramSocket.hpp"

#include <chrono>
#include <string>
#include <thread>
#include <unordered_set>

auto main() -> int
{
  UnixDgramSocket server;
  const std::string server_path = "/tmp/plazza_server";

  if (!server.bind(server_path)) {
    Log::failed << "Server failed to bind";
    return 1;
  }

  std::unordered_set<std::string> client_paths;

  Log::info << "Server waiting for 2 clients...";

  while (client_paths.size() < 2) {
    auto result = server.receive();
    if (result) {
      const auto &[msg, from] = *result;
      Log::info << "Received: '" << msg << "' from " << from;
      if (msg.starts_with("hello"))
        client_paths.insert(from);
    }
  }

  Log::info << "All clients connected. Waiting 1s...";
  std::this_thread::sleep_for(std::chrono::seconds(1));

  for (const auto &client: client_paths) {
    if (!server.send_to(client, "done")) {
      Log::failed << "Fail to send";
      continue;
    }
    Log::info << "Sent 'done' to " << client;
  }

  Log::info << "Server finished.";
  return 0;
}
