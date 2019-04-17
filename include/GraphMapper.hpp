#ifndef GRAPHMAPPER_HPP
#define GRAPHMAPPER_HPP

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include "lemon/smart_graph.h"
#include "lemon/dijkstra.h"

#include <set>
#include <utility>
#include <iostream>
#include <string>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <thread>
#include <mutex>

typedef std::unordered_map <int, std::vector <std::pair <std::string, std::string>>> graphPayload;

template <typename T, typename D>
inline std::ostream& operator<<(std::ostream& os, 
    std::vector< std::pair <T, D> > &lst);

template <typename K, typename P, typename S>
inline std::ostream& operator<<(std::ostream& os, 
    std::unordered_map <K, std::vector <std::pair <P, S>>> &mlst);

class GraphMapper final
{
public:

  ////Vertex data structure
  //struct VertexData
  //{
  //  std::string server_name;
  //  int server_key;
  //};

  ////edges data structure
  //struct EdgeData
  //{
  //  std::string edge_name;
  //  int qkd_key;
  //};

  struct Arc
  {
      std::string sourceID;
      std::string targetID;
      double cost;
  };

  lemon::SmartDigraph ServersGraph;

  GraphMapper() = default;
  ~GraphMapper() = default;
  GraphMapper(const graphPayload &&servers_data);
  GraphMapper(GraphMapper &&) = delete;
  GraphMapper(const GraphMapper&) = delete;
  GraphMapper& operator=(const GraphMapper&) = delete;

  //use Djkstra algorithm with positive QKD keys as weights and:
  //show path table, generated dot file of graph
  void get_shortest_path();

  //returns actual pathes table
  const graphPayload get_actual_table();

  //returns actual graph
  const std::string get_graph();

private:
  //public map to store updated path table by Djkstra algorithm
  graphPayload actual_pathes;

  //table of pathes
  graphPayload table_of_pathes;

  //count of nodes ("servers" pairs with key values)
  size_t nodes_count;

  //graph representation as string
  std::string graph_data;
};

#endif //GRAPHMAPPER_HPP
