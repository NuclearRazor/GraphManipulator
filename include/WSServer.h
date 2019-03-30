//wsserver.h
/*

This header include WSServer class methods declaration

*/

#ifndef WSSERVER_H
#define WSSERVER_H

#define _CRT_SECURE_NO_WARNINGS

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


#endif
