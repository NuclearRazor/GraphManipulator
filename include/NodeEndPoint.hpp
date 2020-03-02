#ifndef NODEENDPOINT_HPP
#define NODEENDPOINT_HPP

#include <NodeMiddleWare.hpp>


class NodeEndPoint final{

  std::string graph_data{};
  NodeObserver* payload_observer;
  nlohmann::json loc_json{};

  const unsigned int port;

public:
  explicit NodeEndPoint(unsigned int port);
  NodeEndPoint(NodeEndPoint &&) = delete;
  NodeEndPoint(const NodeEndPoint&) = delete;
  NodeEndPoint& operator=(const NodeEndPoint&) = delete;
  NodeEndPoint& operator=(const NodeEndPoint&&) = delete;
 ~NodeEndPoint();

  void init();

  void update_payload(const char* const message);
  void dump_payload(const char* const message);

  void call_graph_mapper();

  void notify();
};

#endif //NODEENDPOINT_HPP
