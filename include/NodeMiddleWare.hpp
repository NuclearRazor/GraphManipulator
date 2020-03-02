#ifndef NODEMIDDLEWARE_HPP
#define NODEMIDDLEWARE_HPP

#include "client_ws.hpp"
#include "server_ws.hpp"

#include <future>
#include <json.hpp>
#include <iostream>
#include <future>
#include <mutex>
#include <thread>
#include <chrono>

#include <NodeObserver.hpp>


class NodeMiddleWare final{

  NodeObserver* payload_observer;
  nlohmann::json loc_json{};
  const unsigned int port;

public:
  explicit NodeMiddleWare(unsigned int port);
  NodeMiddleWare(NodeMiddleWare &&) = delete;
  NodeMiddleWare(const NodeMiddleWare&) = delete;
  NodeMiddleWare& operator=(const NodeMiddleWare&) = delete;
  NodeMiddleWare& operator=(const NodeMiddleWare&&) = delete;
 ~NodeMiddleWare();

  void init();

  void update_payload(const char* const message);
  void dump_payload(const char* const message);

  void notify();
};

#endif //NODEMIDDLEWARE_HPP
