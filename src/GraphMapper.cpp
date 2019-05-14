#ifndef GRAPHMAPPER_CPP
#define GRAPHMAPPER_CPP

#include <GraphMapper.hpp>

GraphMapper::GraphMapper(const graphPayload &&servers_data)
{
  //passed (or generated) servers names and it's weights of nodes assign to private member of class - table of pathes
  table_of_pathes = servers_data;

  //count of nodes it is a count of pairs
  nodes_count = servers_data.size();
}

//TODO
//think about parts of processing data
//what data look most of all
void GraphMapper::get_shortest_path()
{
    /*--------------------------GRAPH SHORTEST PATH FIND START-----------------------*/

    lemon::ListGraph::EdgeMap <double> costMap(ServersGraph);
    lemon::ListGraph::NodeMap <std::string> nodeMap(ServersGraph);

    std::map <std::string, int> nodes;
    unsigned long g_idx = 0;
    for (auto &it_table_data : table_of_pathes)
    {
        auto path_node = it_table_data.second;

        for (auto &s_node : path_node)
        {
            //minimal translation, because initial structure have only adjency data
            //not information about sequence of nodes as set
            if (nodes.count(s_node.first) == 0)
            {
                nodes.emplace(std::make_pair(s_node.first, g_idx));
                ++g_idx;
            }
            
            if (nodes.count(s_node.second) == 0)
            {
                nodes.emplace(std::make_pair(s_node.second, g_idx));
                ++g_idx;
            }
                                           
        }
    }

    std::vector<Arc> arcs;
    for (const auto &it_table_data : table_of_pathes)
    {
      auto path_node = it_table_data.second;
      //fill each edge (k) with first vertex (i) and second (i + 1)
      //and assign to the current edge weight, that was generated
      for (const auto &s_node : path_node)
      {
          std::cout << "node: " << s_node.first << " - node: " << s_node.second << " with key = " << it_table_data.first << "\n";
          arcs.emplace_back(Arc{ s_node.first, s_node.second, static_cast<double>(it_table_data.first) });
      }
    }

    //populate graph
    //nodes first
    lemon::ListGraph::Node currentNode;
    for (auto nodesIter = nodes.begin(); nodesIter != nodes.end(); ++nodesIter)
    {
        currentNode = ServersGraph.addNode();
        std::string key = nodesIter->first;
        nodeMap[currentNode] = key;
    }

    //then the arcs with the costs through the cost map
    lemon::ListGraph::Edge currentArc;

    for (auto arcsIter = arcs.begin(); arcsIter != arcs.end(); ++arcsIter)
    {
        int sourceIndex = nodes.at(arcsIter->sourceID);
        int targetIndex = nodes.at(arcsIter->targetID);

        lemon::ListGraph::Node sourceNode = ServersGraph.nodeFromId(sourceIndex);
        lemon::ListGraph::Node targetNode = ServersGraph.nodeFromId(targetIndex);

        currentArc = ServersGraph.addEdge(sourceNode, targetNode);
        costMap[currentArc] = arcsIter->cost;
    }

    std::stringstream graph_stream;
    graph_stream << "digraph D {\n"
        << "  rankdir=LR\n"
        << "  size=\"4,3\"\n"
        << "  ratio=\"fill\"\n"
        << "  edge[dir=\"none\", style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

    for (auto p = arcs.cbegin(); p != arcs.cend(); p++)
    {
        graph_stream << p->sourceID << " -> " << p->targetID << "[label=\"" << p->cost << "\"";

        if (p->sourceID == p->targetID)
            graph_stream << ", color=\"black\"";
        else
            graph_stream << ", color=\"blue\"";

        graph_stream << "]";
    }

    graph_stream << "}";
    std::cout << "Final serialization in sstream: \n" << graph_stream.str() << "\n";

    //TODO
    //must be wrapped and filled by client middlestone
    const std::string start_point = nodes.begin()->first;
    const std::string target_point = nodes.rbegin()->first;

    std::cout << "Start point: " << start_point << "\n";
    std::cout << "Target point: " << target_point << "\n";

    //add source & target
    lemon::ListGraph::Node startN = ServersGraph.nodeFromId(nodes.at(start_point));
    lemon::ListGraph::Node endN = ServersGraph.nodeFromId(nodes.at(target_point));

    //defining the type of the Dijkstra Class
    using SptSolver = lemon::Dijkstra<lemon::ListGraph, lemon::ListGraph::EdgeMap<double>>;
    SptSolver spt(ServersGraph, costMap);
    spt.run(startN, endN);

    /* Walk in whole SPT is possible from specified orig and end
       but dest must be part of the SPT and
       an orig node must not be a dest node */
    std::vector <lemon::ListGraph::Node> path;

    for (lemon::ListGraph::Node v = endN; v != startN; v = spt.predNode(v))
    {
        if (v != lemon::INVALID && spt.reached(v)) //special LEMON node constant
        {
            path.emplace_back(v);
        }
    }

    path.emplace_back(startN);

    double cost = spt.dist(endN);

    //print out the path with reverse iterator
    std::cout << "Shortest Path from " << start_point << " to " << target_point << " is: " << std::endl;

    for (auto p = path.rbegin(); p != path.rend(); ++p)
    {
        std::cout << nodeMap[*p] << " " << spt.dist(*p) << std::endl;
        std::cout << "^" << " " << " " << std::endl;
        std::cout << "|" << " " << " " << std::endl;
        std::cout << "v" << " " << " " << std::endl;
    }

    std::cout << "\nTotal cost for the shortest path is: " << cost << std::endl;

    graph_data = graph_stream.str();
}

const graphPayload GraphMapper::get_actual_table()
{
  return actual_pathes;
}

const std::string GraphMapper::get_graph()
{
  return graph_data;
}

#endif // GRAPHMAPPER_CPP
