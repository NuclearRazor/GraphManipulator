#ifndef WSSERVER_CPP
#define WSSERVER_CPP

#include <wsserver.hpp>

WSServer::WSServer(const unsigned int port)
{
  uWS::Hub h;

  //listen port 
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << "\n";

    //uWs Hub istance process on each current callable for weboskcet instance
    //process on items by lambdas
    h.onMessage([&](uWS::WebSocket<uWS::SERVER> *ws, char *msg, size_t length, uWS::OpCode opCode)
    {
      update_payload(msg, length);
      ws->send(graph_data.c_str(), graph_data.length(), opCode);
    });

    //process on message
    h.onConnection([&](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req)
    {
      std::cout << "Connected!" << std::endl;
    });

    //close weboskcet connection
    h.onDisconnection([&](uWS::WebSocket<uWS::SERVER> *ws, int code, char *msg, size_t length)
    {
      ws->close();
      std::cout << "Disconnected!" << "\n";
    });
  }
  else
  {
    std::cerr << "Failed to listen to port" << "\n";
  }

  h.run();

}

//update payload of graph parameters
//set parameters of graph as int values into vector of int's
void WSServer::update_payload(const char* const message, size_t length)
{

  try
  {
      nlohmann::json json_obj = nlohmann::json::parse(std::string(message, length));

      //resize vector with 3 elements as graph options parameters
      payload_data.reserve(json_obj.size()); //allocate
      payload_data.resize(json_obj.size()); //make constant container size

      for (std::pair<msg_it, payload_it> i(json_obj.cbegin(), payload_data.begin());
          i.first != json_obj.end()  && i.second != payload_data.end();
          ++i.first, ++i.second)
      {
          (*i.second) = (*i.first);
      }

      call_graph_mapper();

  }
  catch (...)
  {
      throw std::runtime_error("Cannot process on JSON");
  }

}

//input: graph options - scalars of int's
//output: graph as std::string object
void WSServer::call_graph_mapper()
{
    std::unique_ptr<GraphProcessor> UGraphProcessorInstance = std::make_unique<GraphProcessor>();
    graph_data = std::move(UGraphProcessorInstance->serialize_graph(std::move(payload_data)));
}

#endif //WSSERVER_CPP
