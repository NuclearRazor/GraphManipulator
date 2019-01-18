#ifndef PATHMAPPER_H
#define PATHMAPPER_H

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <boost/config.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/adaptor/strided.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <utility>
#include <iostream>
#include <string>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#include <memory>
#include <thread>
#include <mutex>
#include <future>


template <typename T, typename D>
inline std::ostream& operator<<(std::ostream& os, 
    std::vector< std::pair <T, D> > &lst);

template <typename K, typename P, typename S>
inline std::ostream& operator<<(std::ostream& os, 
    std::map <K, std::vector <std::pair <P, S>>> &mlst);

class GraphMapper final
{
public:

  //public map to store updated path table by Djkstra algorithm
  std::map < int, std::vector < std::pair <std::string, std::string> > > actual_pathes;

  //Vertex data structure
  struct VertexData
  {
    std::string server_name;
    int server_key;
  };


  //edges data structure
  struct EdgeData
  {
    std::string edge_name;
    int qkd_key;
  };


  //Servers graph
  typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS, //undirected graph
    VertexData, //storage mechanism to store vertex properties
    boost::property<boost::edge_weight_t, int, EdgeData> //boost::property<boost::edge_weight_t, int>
  > ServersGraph;

  GraphMapper() = default;
  ~GraphMapper() = default;
  GraphMapper(std::map <int, std::vector <std::pair <std::string, std::string>>> &servers_data);
  GraphMapper(GraphMapper &&) = delete;
  GraphMapper(const GraphMapper&) = delete;
  GraphMapper& operator=(const GraphMapper&) = delete;

  /*

  Method use Djkstra algorithm with positive QKD keys as weights and:
  show path table, generated dot file of graph

  */
  void get_shortest_path();

  //returns actual pathes table
  std::map< int, std::vector < std::pair <std::string, std::string> > >
  get_actual_table();

  //returns actual graph
  std::string get_graph();

private:

  //table of pathes
  std::map <int, std::vector < std::pair <std::string, std::string> >> table_of_pathes;

  //count of nodes ("servers" pairs with key values)
  size_t nodes_count;

  //graph representation as string
  std::string graph_data;

};

#endif
