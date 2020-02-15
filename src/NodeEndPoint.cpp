#ifndef NODEENDPOINT_CPP
#define NODEENDPOINT_CPP

#include <NodeEndPoint.hpp>


NodeEndPoint::NodeEndPoint(unsigned int port){

  uWS::Hub h;
  payload_observer = new NodeObserver();

  h.onPing([&](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length) {
    std::cout << std::string(message, length) << std::endl;
    //ws -> nodemiddleware
  });

  h.onMessage([&](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
    std::cout << std::string(message, length) << std::endl;
    //ws->send(message, length, opCode);
  });

  h.onConnection([&](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
    std::cout << "\nNodeEndPoint Connected!" << std::endl;
  });

  h.onDisconnection([&](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *msg, size_t length) {
    h.getDefaultGroup<uWS::CLIENT>().close();
    std::cout << "\nNodeEndPoint Disconnected!" << std::endl;
  });

  h.connect("ws://localhost:" + std::to_string(port), nullptr);
  h.run();
}


void NodeObserver::update(const nlohmann::json& payload) {
  o_payload = payload;
}


void NodeEndPoint::dump_payload(const char* const message, size_t length) {
  if (message)
    actual_payload = nlohmann::json::parse(std::string(message, length));

  if (actual_payload["ok"] == "true" || actual_payload["ok"])
    std::cout << "\njson validated" << std::endl;

  notify();
}


void NodeEndPoint::update_payload(const char* const message, size_t length){
  try{
    dump_payload(message, length);
  }
  catch (std::logic_error){
      throw std::runtime_error("Cannot deserialize JSON data");
  }
  catch (...){
      throw std::runtime_error("Unknown exception during JSON processing");
  }
}


void NodeEndPoint::notify() {
  payload_observer->update(std::move(actual_payload));
}

#endif //NODEENDPOINT_CPP
