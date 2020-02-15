#ifndef NODEENDPOINT_HPP
#define NODEENDPOINT_HPP

#include <NodeMiddleWare.hpp>


class NodeEndPoint final{

  std::string graph_data{};
  uWS::Hub h;
  NodeObserver* payload_observer;
  nlohmann::json local_payload;

public:
  explicit NodeEndPoint(unsigned int port);
  NodeEndPoint(NodeEndPoint &&) = delete;
  NodeEndPoint(const NodeEndPoint&) = delete;
  NodeEndPoint& operator=(const NodeEndPoint&) = delete;
  NodeEndPoint& operator=(const NodeEndPoint&&) = delete;
 ~NodeEndPoint() = default;

  void update_payload(const char* const message, size_t length);
  void dump_payload(const char* const message, size_t length);

  void call_graph_mapper();

  void notify();
};

#endif //NODEENDPOINT_HPP
