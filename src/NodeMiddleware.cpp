#ifndef NODEMIDDLEWARE_CPP
#define NODEMIDDLEWARE_CPP

#include "../include/NodeMiddleware.hpp"


NodeMiddleWare::NodeMiddleWare(unsigned int p_port) : port(p_port){
  init();
}


NodeMiddleWare::~NodeMiddleWare(){
  payload_observer->~NodeObserver();
}
 

void NodeMiddleWare::init(){
  using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
  using WsConnection = std::shared_ptr<WsServer::Connection>;

  payload_observer = new NodeObserver();
  
  WsServer server;
  server.config.port = port;

  auto &global_endpoint = server.endpoint["^/set_mapper/"];
  global_endpoint.on_message = [&](WsConnection connection, std::shared_ptr<WsServer::InMessage> in_message) {
    auto out_message = in_message->string();

    std::cout << "NodeMiddleWare: Message received: \"" << out_message << "\" from " << connection.get() << std::endl;

    update_payload(out_message.c_str());

    //TODO
    //send payload to UI if digraph/... object state updated only

    //std::cout << "Server: Sending message \"" << out_message << "\" to " << connection.get() << std::endl;
    //connection->send(out_message, [](const SimpleWeb::error_code &ec) {
    //  if(ec) {
    //    std::cout << "Server: Error sending message. " <<
    //      "Error: " << ec << ", error message: " << ec.message() << std::endl;
    //  }
    //});
  };

  global_endpoint.on_open = [](WsConnection connection) {
    std::cout << "NodeMiddleWare: Opened connection " << connection.get() << std::endl;
  };

  global_endpoint.on_close = [&](WsConnection connection, int status, const std::string & reason) {
    std::cout << "NodeMiddleWare: Closed connection " << connection.get() 
      << " with status code " << status << " and reason: " << reason << std::endl;
    delete payload_observer;
  };

  global_endpoint.on_handshake = [](WsConnection connection, SimpleWeb::CaseInsensitiveMultimap & response_header) {
    return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
  };

  global_endpoint.on_error = [](WsConnection connection, const SimpleWeb::error_code &ec) {
    std::cout << "NodeMiddleWare: Error in connection " << connection.get() << ". "
      << "Error: " << ec << ", error message: " << ec.message() << std::endl;
  };

  auto &data_endpoint = server.endpoint["^/set_mapper/data/"];
  data_endpoint.on_message = [&](WsConnection connection, std::shared_ptr<WsServer::InMessage> in_message) {

    std::cout << "get message: " << in_message->string() << "\n";

    auto const _payload = payload_observer->get_payload();

    if(!_payload.empty() && !_payload.is_null())
    {
      auto const out_message = std::make_shared<std::string>(payload_observer->get_payload().dump());
      connection->send(*out_message, [](const SimpleWeb::error_code &ec) {
        if(ec) {
          std::cout << "NodeMiddleWare: Error sending message. " <<
            "Error: " << ec << ", error message: " << ec.message() << std::endl;
        }
      });
    }
    else{
      auto const out_message = std::make_shared<std::string>("{\"ok\": \"true\", \"message\": \"no data to redner\"}");
      connection->send(*out_message, [](const SimpleWeb::error_code &ec) {
        if(ec) {
          std::cout << "NodeMiddleWare: Error sending message. " <<
            "Error: " << ec << ", error message: " << ec.message() << std::endl;
        }
      });
    }
  };

  server.start();
}


void NodeMiddleWare::dump_payload(const char* const msg) {

  if (msg)
    loc_json = nlohmann::json::parse(msg);

  if (loc_json["ok"] == "true" || loc_json["ok"])
    std::cout << "\njson validated\n";

  notify();
}


void NodeMiddleWare::update_payload(const char* const msg){
  try{
    dump_payload(msg);
  }
  catch (std::logic_error){
      throw std::runtime_error("Cannot deserialize JSON data");
  }
  catch (...){
      throw std::runtime_error("Unknown exception during JSON processing");
  }
}


void NodeMiddleWare::notify() {
  payload_observer->update(std::move(loc_json));
}

#endif //NODEMIDDLEWARE_CPP
