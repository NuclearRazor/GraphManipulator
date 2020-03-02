#ifndef NODEOBSERVER_HPP
#define NODEOBSERVER_HPP

#include <json.hpp>


class NodeObserver final {

  nlohmann::json o_payload;

public:
  NodeObserver() = default;
  NodeObserver(NodeObserver &&) = delete;
  NodeObserver(const NodeObserver&) = delete;
  NodeObserver& operator=(const NodeObserver&) = delete;
  NodeObserver& operator=(const NodeObserver&&) = delete;
  ~NodeObserver() = default;

  void update(const nlohmann::json&& payload);
  const nlohmann::json get_payload();
};

#endif //NODEOBSERVER_HPP
