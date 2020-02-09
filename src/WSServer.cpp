#ifndef WSSERVER_CPP
#define WSSERVER_CPP

#include <wsserver.hpp>

WSServer::WSServer(unsigned int port){

  if (h.listen(port)){
    std::cout << "Listening to port: " << port << "\n";

    h.onMessage([&](uWS::WebSocket<uWS::SERVER> *ws, char *msg, size_t length, uWS::OpCode opCode) {
      update_payload(msg, length);
      ws->send(graph_data.c_str(), graph_data.length(), opCode);
    });

    h.onConnection([&](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req){
      std::cout << "\nConnected!\n";
    });

    h.onDisconnection([&](uWS::WebSocket<uWS::SERVER> *ws, int code, char *msg, size_t length){
      ws->close();
      std::cout << "\nDisconnected!\n";
    });
  }
  else{
    std::cerr << "\nFailed to listen to port" << "\n";
  }

  h.run();
}


void WSServerObserver::update(const nlohmann::json&& payload) {
  o_payload = payload;
}


void WSServer::dump_payload(const char* const message, size_t length) {
  if (message)
    actual_payload = nlohmann::json::parse(std::string(message, length));

  if (actual_payload["ok"] == "true" || actual_payload["ok"])
    std::cout << "\njson validated\n";

  notify();
}


void WSServer::update_payload(const char* const message, size_t length){
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

void WSServer::notify() {
  std::cout << "notify";
  payload_observer.update(std::move(actual_payload));
}

#endif //WSSERVER_CPP
