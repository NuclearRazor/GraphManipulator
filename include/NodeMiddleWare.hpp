#ifndef NODEMIDDLEWARE_HPP
#define NODEMIDDLEWARE_HPP

#include <uWS.h>
#include <json.hpp>
#include <iostream>
#include <future>
#include <mutex>
#include <thread>
#include <chrono>


static thread_local nlohmann::json actual_payload{};


class NodeObserver final {

  nlohmann::json o_payload;

public:
  NodeObserver() = default;
  NodeObserver(NodeObserver &&) = delete;
  NodeObserver(const NodeObserver&) = delete;
  NodeObserver& operator=(const NodeObserver&) = delete;
  NodeObserver& operator=(const NodeObserver&&) = delete;
  ~NodeObserver() = default;

  void update(const nlohmann::json& payload);
  const nlohmann::json get_payload();
};

//TODO
//make inherited from NodeController interface
class NodeMiddleWare final{

  uWS::Hub h;
  NodeObserver* payload_observer;

public:
  explicit NodeMiddleWare(unsigned int port);
  NodeMiddleWare(NodeMiddleWare &&) = delete;
  NodeMiddleWare(const NodeMiddleWare&) = delete;
  NodeMiddleWare& operator=(const NodeMiddleWare&) = delete;
  NodeMiddleWare& operator=(const NodeMiddleWare&&) = delete;
 ~NodeMiddleWare() = default;

  void update_payload(const char* const message, size_t length);
  void dump_payload(const char* const message, size_t length);

  void notify();
};

#endif //NODEMIDDLEWARE_HPP
