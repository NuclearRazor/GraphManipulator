#ifndef NODEENDPOINT_CPP
#define NODEENDPOINT_CPP

#include <NodeEndPoint.hpp>


NodeEndPoint::NodeEndPoint(unsigned int p_port) : port(p_port){
  init();
}


NodeEndPoint::~NodeEndPoint(){
  payload_observer->~NodeObserver();
}


void NodeEndPoint::init(){

  using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;
  using WsConnection = std::shared_ptr<WsClient::Connection>;

  using namespace std::chrono_literals;

  payload_observer = new NodeObserver();

  auto const client_endpoint = "localhost:" + std::to_string(port) + "/set_mapper/data/";

  while(true){
    std::atomic<bool> closed(false);
    WsClient client(client_endpoint);

    client.on_message = [](WsConnection connection, std::shared_ptr<WsClient::InMessage> in_message) {
      std::cout << "NodeEndPoint: Message received: \"" << in_message->string() << std::endl;

      std::string out_message("{\"ok\": \"true\", \"data\": \"updated data\"}");
      std::this_thread::sleep_for(250ms);

      connection->send(out_message);
    };

    client.on_open = [](std::shared_ptr<WsClient::Connection> connection) {
      std::string out_message("{\"ok\": \"true\", \"message\": \"no data to redner\"}");
      connection->send(out_message);
    };

    client.on_close = [&](WsConnection connection, int status, const std::string & reason) {
      std::cout << "NodeEndPoint: Closed connection with status code " << status << std::endl;
      delete payload_observer;
      closed = true;
    };

    client.on_error = [](WsConnection connection, const SimpleWeb::error_code &ec) {
      std::cout << "NodeEndPoint: Error: " << ec << ", error message: " << ec.message() << std::endl;
    };

    std::thread client_thread([&client]() {
      client.start();
    });

    while(!closed)
      std::this_thread::sleep_for(250ms);

    client.stop();
    client_thread.join();
  }
}


void NodeEndPoint::dump_payload(const char* const message) {
  if (message)
    loc_json = nlohmann::json::parse(message);

  if (loc_json["ok"] == "true" || loc_json["ok"])
    std::cout << "\njson validated" << std::endl;;

  notify();
}


void NodeEndPoint::update_payload(const char* const message){
  try{
    dump_payload(message);
  }
  catch (std::logic_error){
      throw std::runtime_error("Cannot deserialize JSON data");
  }
  catch (...){
      throw std::runtime_error("Unknown exception during JSON processing");
  }
}


void NodeEndPoint::notify() {
  payload_observer->update(std::move(loc_json));
}

#endif //NODEENDPOINT_CPP
