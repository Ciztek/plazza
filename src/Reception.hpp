#pragma once

#include <sched.h>
#include <thread>

#include "ErrorOr.hpp"
#include "Kitchen.hpp"
#include "KitchenCatalog.hpp"
#include "reception/TicketFacade.hpp"

class Reception {
public:
  Reception(const KitchenCalatog &catalog, size_t ncook);

  void run_repl(bool interactive);
  auto create_kitchen(int id, int port) -> ErrorOr<Kitchen>;

  auto dispatch_order(PizzaOrder &order) -> ErrorOr<int>;
  void send_to_kitchen(int kitchen_id, const std::string &message);
  void send_to_all_kitchens(const std::string &message);

private:
  std::vector<pid_t> _kitchen_ids;
  std::jthread _kitchen_thread;
  std::mutex _kitchen_mutex;

  KitchenCalatog _catalog;
  size_t _ncook;
};
