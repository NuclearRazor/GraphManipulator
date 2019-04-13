#ifndef GRAPHPROCESSOR_CPP
#define GRAPHPROCESSOR_CPP

#include "GraphProcessor.hpp"

std::mutex mtx;

std::string GraphProcessor::serialize_graph(const std::vector <int>&& _payload)
{
    std::lock_guard<std::mutex> guard(mtx);

    std::unique_ptr<AdjacencyObjectsGenerator> UAdjacencyObjects = std::make_unique<AdjacencyObjectsGenerator>(_payload[0], _payload[1], _payload[2]);
    graphPayload _buf_servers_data;

    std::thread task_generate([&] {_buf_servers_data = UAdjacencyObjects->get_adjency_objects(); });

    if (task_generate.joinable())
    {
        task_generate.join();
    }

    std::unique_ptr<GraphMapper> UGraphMapper = std::make_unique<GraphMapper>(std::move(_buf_servers_data));
    std::thread task_processpath([&] { UGraphMapper->get_shortest_path(); });

    if (task_processpath.joinable())
    {
        task_processpath.join();
    }

    std::string graph_datastring;
    std::thread task_getpath([&] { graph_datastring = UGraphMapper->get_graph(); });

    if (task_getpath.joinable())
    {
        task_getpath.join();
    }

    return graph_datastring;
}

#endif
