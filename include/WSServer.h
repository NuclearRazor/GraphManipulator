//wsserver.h
/*

This header include WSServer class methods declaration

*/

#ifndef WSSERVER_H
#define WSSERVER_H

#define _CRT_SECURE_NO_WARNINGS

#include <uWS/uWS.h>
#include "json.hpp"
#include <atomic>
#include <memory>
#include <thread>
#include <mutex>
#include <future>

class WSServer
{

public:

  WSServer(unsigned int port);
  WSServer(WSServer &&) = default;
  WSServer(const WSServer&) = default;
  WSServer& operator=(const WSServer&) = default;
  ~WSServer() = default;

  void update_payload(char *message, size_t length);
  void call_graph_mapper();

private:

  std::vector <int> payload_data;
  std::string graph_data;
  GraphProcessor* task_process;

};


#endif
