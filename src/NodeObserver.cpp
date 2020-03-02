#ifndef NODEOBSERVER_CPP
#define NODEOBSERVER_CPP

#include "../include/NodeObserver.hpp"


void NodeObserver::update(const nlohmann::json&& payload) {
  if(!payload.empty() && !payload.is_null())
    o_payload = payload;
}


const nlohmann::json NodeObserver::get_payload() {
  return o_payload;
}


#endif //NODEOBSERVER_CPP
