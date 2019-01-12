//GraphProcessor.cpp
/*

This header include graphprocessor class methods definitions

*/

#ifndef GRAPHPROCESSOR_CPP
#define GRAPHPROCESSOR_CPP

#include "../include/GraphProcessor.h"


std::string GraphProcessor::serialize_graph(const std::vector <int>& _payload)
{
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(15ms);

    for (auto &v_el : _payload)
    {
        std::cout << v_el << "\n";
    }

    AdjacencyObjectsGenerator* task_generate = new AdjacencyObjectsGenerator(_payload[1], _payload[0], _payload[2]);
    std::map <int, std::vector < std::pair <std::string, std::string> > > _buf_servers_data;
    std::thread task_thread_generate([&] {_buf_servers_data = task_generate->get_adjency_objects(); });

    //task_thread_generate.detach();
    if (task_thread_generate.joinable())
    {
        task_thread_generate.join();
    }

    std::this_thread::sleep_for(1ms);

    GraphMapper* task_execute_table = new GraphMapper(_buf_servers_data);
    std::thread task_thread_execute(&GraphMapper::get_shortest_path, std::move(task_execute_table));

    //task_thread_execute.detach();
    if (task_thread_execute.joinable())
    {
        task_thread_execute.join();
    }

    std::this_thread::sleep_for(2ms);

    std::string graph_datastring;
    std::thread task_thread_getgraph([&] {graph_datastring = task_execute_table->get_graph(); });

    //task_thread_getgraph.detach();
    if (task_thread_getgraph.joinable())
    {
        task_thread_getgraph.join();
    }

    return graph_datastring;
}

#endif
