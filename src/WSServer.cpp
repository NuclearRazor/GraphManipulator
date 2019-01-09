//WSServer.cpp
/*

This header include wsserver class methods definitions

*/

#ifndef WSSERVER_CPP
#define WSSERVER_CPP

#include "../include/WSServer.h"


WSServer::WSServer(unsigned int port)
{

  //resize vector with 3 elements as graph options parameters
  payload_data.resize(3);

  uWS::Hub h;

  //listen port 
  if (h.listen(port))
  {

    std::cout << "Listening to port " << port << "\n";


    //uWs Hub istance process on each current callable for weboskcet instance
    //process on items by lambdas
    h.onMessage([&](uWS::WebSocket<uWS::SERVER> *ws, char *msg, size_t length, uWS::OpCode opCode)
    {
      //update_payload(msg, length);

      //std::lock_guard <std::mutex> guard(mtx);
      using namespace std::literals::chrono_literals;

      std::thread task_thread_serialize(&WSServer::update_payload, this, std::ref(msg), std::ref(length));

      //task_thread_serialize.detach();
      if (task_thread_serialize.joinable())
      {
          task_thread_serialize.join();
      }

      std::this_thread::sleep_for(5ms);

      ws->send(graph_data.c_str(), graph_data.length(), opCode);

    });


    //process weboskcet message if it exist and called
    //and update Hub object - &h
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
void WSServer::update_payload(char *message, size_t length)
{
  using json = nlohmann::json;

  try
  {
      std::cout << "call update\n";
      json json_obj = json::parse(std::string(message, length));

      std::cout << json_obj["matrix_dim"] << "\n";
      std::cout << json_obj["characters_length"] << "\n";
      std::cout << json_obj["metrics"] << "\n";

      payload_data[0] = int(json_obj["matrix_dim"]);
      payload_data[1] = int(json_obj["characters_length"]);
      payload_data[2] = int(json_obj["metrics"]);

      using namespace std::literals::chrono_literals;

      std::thread task_thread_map(&WSServer::call_graph_mapper, this);

      //task_thread_map.detach();
      if (task_thread_map.joinable())
      {
          task_thread_map.join();
      }

      std::this_thread::sleep_for(3ms);
  }
  catch (...)
  {
      throw std::runtime_error("Cannot process on JSON");
  }

}


//input: graph options - scalars of int's
//output: graph dot file - std::string object
void WSServer::call_graph_mapper()
{
  std::cout << "call call_graph_mapper\n";
  using namespace std::literals::chrono_literals;

  std::thread task_thread_map([&] {graph_data = serialize_graph(payload_data); });

  //task_thread_map.detach();
  if (task_thread_map.joinable())
  {
      task_thread_map.join();
  }

  std::this_thread::sleep_for(2ms);
}


#endif
