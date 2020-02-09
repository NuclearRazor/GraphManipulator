#ifndef WSSERVER_HPP
#define WSSERVER_HPP

#include <uWS.h>
#include <json.hpp>
#include <iostream>
#include <future>
#include <mutex>
#include <thread>

static nlohmann::json actual_payload{};

class WSServerObserver final {

  nlohmann::json o_payload;

public:
  WSServerObserver() = default;
  WSServerObserver(WSServerObserver &&) = delete;
  WSServerObserver(const WSServerObserver&) = delete;
  WSServerObserver& operator=(const WSServerObserver&) = delete;
  ~WSServerObserver() = default;

  void update(const nlohmann::json&& payload);
  nlohmann::json get_payload();
};


class WSServer final{

  std::string graph_data{};
  uWS::Hub h;
  WSServerObserver payload_observer;

public:
  explicit WSServer(unsigned int port);
  WSServer(WSServer &&) = delete;
  WSServer(const WSServer&) = delete;
  WSServer& operator=(const WSServer&) = delete;
 ~WSServer() = default;

  void update_payload(const char* const message, size_t length);
  void dump_payload(const char* const message, size_t length);

  void call_graph_mapper();

  void notify();
};

#endif //WSSERVER_HPP
