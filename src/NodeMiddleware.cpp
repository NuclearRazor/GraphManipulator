#ifndef NODEMIDDLEWARE_CPP
#define NODEMIDDLEWARE_CPP

#include <NodeMiddleWare.hpp>


NodeMiddleWare::NodeMiddleWare(unsigned int port){

  uWS::Hub h;
  std::mutex m; //~
  using namespace std::chrono_literals;
  payload_observer = new NodeObserver();


  h.onPong([&](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length) {
    if (!actual_payload.empty()) {
      m.lock();
      auto test_payload = payload_observer->get_payload();
      std::string _payload = actual_payload.dump();

      //const char* _payload
      //const char* const _payload
      //std::string -> c_str() _payload

      std::cout << "send to nodeendpoint: " << _payload << std::endl;
      ws->send(_payload.c_str(), uWS::OpCode::BINARY);
      actual_payload.clear();
      m.unlock();
    }
  });

  h.onMessage([&](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    update_payload(message, length);
    std::cout << std::string(message, length) << std::endl;

    //TODO
    //update model
    //receive message from NodeEndClient
    ws->send(message, length, opCode);
  });

  h.onConnection([&](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req){
    std::cout << "\nMiddleWare Connected!" << std::endl;
  });

  h.onDisconnection([&](uWS::WebSocket<uWS::SERVER> *ws, int code, char *msg, size_t length){
    //ws->close();
    h.getDefaultGroup<uWS::SERVER>().close();
    std::cout << "\nMiddleWare Disconnected!" << std::endl;
  });

  h.listen(port);
  h.getDefaultGroup<uWS::SERVER>().startAutoPing(1000);

  h.run();
}


void NodeObserver::update(const nlohmann::json& payload) {
  o_payload = payload;
}


const nlohmann::json NodeObserver::get_payload() {
  return o_payload;
}


void NodeMiddleWare::dump_payload(const char* const message, size_t length) {
  if (message)
    actual_payload = nlohmann::json::parse(std::string(message, length));

  if (actual_payload["ok"] == "true" || actual_payload["ok"])
    std::cout << "\njson validated\n";

  notify();
}


void NodeMiddleWare::update_payload(const char* const message, size_t length){
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


void NodeMiddleWare::notify() {
  std::cout << "notify";
  payload_observer->update(actual_payload);
}

#endif //NODEMIDDLEWARE_CPP
