#ifndef WSSERVER_HPP
#define WSSERVER_HPP

#include "uWS.h"
#include "json.hpp"

#include <atomic>
#include <memory>
#include <thread>
#include <mutex>
#include <future>

class WSServer final
{
public:
  explicit WSServer(const unsigned int port);
  WSServer(WSServer &&) = delete;
  WSServer(const WSServer&) = delete;
  WSServer& operator=(const WSServer&) = delete;
 ~WSServer() = default;

  void update_payload(const char* const message, size_t length);
  void call_graph_mapper();

private:
  std::vector <int> payload_data;
  std::string graph_data;
  GraphProcessor* task_process;
};

#endif //WSSERVER_HPP
