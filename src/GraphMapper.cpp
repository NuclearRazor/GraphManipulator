#ifndef PATHMAPPER_CPP
#define PATHMAPPER_CPP

#include "../include/GraphMapper.h"


GraphMapper::GraphMapper(std::map <int, std::vector <std::pair <std::string, std::string>>> &servers_data)
{

  std::cout << "\n---Table of nodes:\n" << "\n";

  std::cout << servers_data << "\n";

  //passed (or generated) servers names and it's weights of nodes assign to private member of class - table of pathes
  this->table_of_pathes = servers_data;

  //count of nodes it is a count of pairs
  this->nodes_count = servers_data.size();

}

void GraphMapper::get_shortest_path()
{

  /*--------------------------GRAPH SHORTEST PATH FIND START-----------------------*/

  //GRAPH COUNT OF VERTECIES SIMILAR TO 2*nodes_count
  ServersGraph G(2 * nodes_count + 1);

  std::map < int, std::vector < std::pair <std::string, std::string> > >::iterator it_table_data;

  //counter to indexing G
  unsigned it_graph = 0;

  //iterate over table data to translate servers nodes to vertecies and edges
  for (auto &it_table_data : table_of_pathes)
  {

      auto path_node = it_table_data.second;

      /*
      fill each edge (k)
      with first vertex (i) and second (i + 1)
      and assign to the current edge weight, that was generated
      */
      for (auto &s_node : path_node)
      {
          //add vertex name (i) - server name
          G[it_graph].server_name = s_node.first;

          //add vertex (i) with it's key
          G[it_graph].server_key = it_table_data.first;

          //add vertex (i) to graph G
          auto v1 = boost::add_vertex({ G[it_graph].server_name, G[it_graph].server_key }, G);

          //increase iterator of vertecies
          ++it_graph;

          //add vertex name (i + 1) - server name
          G[it_graph].server_name = s_node.second;

          //add vertex (i + 1) with it's key
          G[it_graph].server_key = it_table_data.first;

          auto v2 = boost::add_vertex({ G[it_graph].server_name, G[it_graph].server_key }, G);

          ////add vertex (i + 1) to graph G
          auto e = boost::add_edge(v1, v2, G).first;

          //add edge name of current edge (k) to graph G
          G[e].edge_name = "QKD";

          G[e].qkd_key = it_table_data.first;

          //increase iterator to get next vertex to filling data
          ++it_graph;

      }

  }

  std::cout << "\n---Vertecies and edges of graph:\n" << "\n";

  for (unsigned int l = 0; l < 2 * nodes_count; ++l)
  {
    ServersGraph::out_edge_iterator eit, eend;

    std::tie(eit, eend) = boost::out_edges(2 * nodes_count + l, G);

    std::for_each(eit, eend, [&G](ServersGraph::edge_descriptor it)
    {

      std::cout << G[boost::target(it, G)].server_name << "\t_____________\t" << G[boost::source(it, G)].server_name
        << "\t<--->\t" << " [" << G[it].edge_name << "] - [" << G[it].qkd_key << "]" << "\n";

      std::cout << "\n";

    }
    );

  }


  /*--------------------------FIND SHORTEST PATH BY DJKSTRA ALGORITHM START-----------------------*/

  typedef boost::property_map < ServersGraph, boost::vertex_index_t >::type IndexMap;
  typedef boost::graph_traits < ServersGraph >::vertex_descriptor Vertex;
  typedef boost::graph_traits < ServersGraph >::vertex_iterator Viter;
  typedef int Weight;

  //START POINT IN DJKSTRA ALGORITHM
  Viter initial = boost::vertices(G).first;

  Vertex s = 0;

  if(!(*initial)) 
  {
      Vertex s = *initial;
  }
  else
  {
      throw std::runtime_error("Cannot get value of first vertex");
  }

  std::vector <Vertex> predecessors(boost::num_vertices(G)); // To store parents nodes
  std::vector <Weight> distances(boost::num_vertices(G)); // To store distances/weights/...

  IndexMap indexMap;

  //DEFINE MAP ITERATOR OVER ALL PARAMETERS OF GRAPH
  typedef boost::iterator_property_map < Vertex*, IndexMap, Vertex, Vertex& > PredecessorMap;
  typedef boost::iterator_property_map < Weight*, IndexMap, Weight, Weight& > DistanceMap;

  PredecessorMap predecessorMap(&predecessors[0], indexMap);
  DistanceMap distanceMap(&distances[0], indexMap);

  if (!(s))
      std::cout << "Warning: initial vertex is equal zero\n";

  //[1] parameter -> graph
  //[2] parameter -> start point (selected vertex)
  //[3] parameter -> graph map properties
  boost::dijkstra_shortest_paths(G, s, boost::distance_map(distanceMap).predecessor_map(predecessorMap));

  /*--------------------------FIND SHORTEST PATH BY DJKSTRA ALGORITHM END-----------------------*/


  std::cout << "\n";

  //SAVE .DOT FILE OF GRAPH
  std::ofstream dot_file("out_graph.gr");

  dot_file << "digraph D {\n"
    << "  rankdir=LR\n"
    << "  size=\"4,3\"\n"
    << "  ratio=\"fill\"\n"
    << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";


  //store vertices integer indexes
  std::vector <Vertex> p(boost::num_vertices(G));

  boost::graph_traits <ServersGraph>::edge_iterator ei, ei_end;

  std::cout << "\n---Path table: \n" << "\n";
  std::cout << "---[start] --> [end]\n" << "\n";

  using VD = ServersGraph::vertex_descriptor;

  VD start_vertex = boost::num_vertices(G);
  VD end_vertex = boost::num_vertices(G);

  auto predmap = predecessors.data(); // interior properties: boost::get(boost::vertex_predecessor, g);
  auto distmap = distances.data();

  size_t distance = distmap[end_vertex];

  std::cout << "Distance from # " << start_vertex << " to # " << end_vertex << ": " << distance << "\n";

  if (distance != size_t(-1))
  {
    std::deque<VD> path;

    for (VD current = end_vertex; current != G.null_vertex() && predmap[current] != current && current != start_vertex;)
    {
      path.push_front(predmap[current]);
      current = predmap[current];
      std::cout << "current vertex: " << predmap[current] << "\n";
    }

    std::cout << "Path from #" << start_vertex << " to #" << end_vertex << ": ";
    std::copy(path.begin(), path.end(), std::ostream_iterator<VD>(std::cout, ", "));

    std::cout << end_vertex << "\n";
  }


  std::vector <ServersGraph::vertex_descriptor> pr_map(boost::num_vertices(G));
  std::vector <double>                   dist_map(boost::num_vertices(G));

  for (auto vd : boost::make_iterator_range(boost::vertices(G)))
  {
    std::cout << "distance(" << vd << ") = " << dist_map[vd] << ", ";
    std::cout << "parent(" << vd << ") = " << pr_map[vd] << std::endl;
  }


  //slize to print out current index of vertex
  unsigned int slize = 2 * nodes_count + 1;

  std::map <int, std::vector < std::pair <std::string, std::string> >> actual_pathes;

  std::vector < std::pair <std::string, std::string> > _buf_pairs;

  //print out path table and store it to .dot file
  for (std::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei)
  {
    boost::graph_traits < ServersGraph >::edge_descriptor e = *ei;
    boost::graph_traits < ServersGraph >::vertex_descriptor u = boost::source(e, G), v = boost::target(e, G);

    //store all finded pathes to public map
    _buf_pairs.push_back(std::make_pair(G[u].server_name, G[v].server_name));
    actual_pathes.insert({ G[e].qkd_key, _buf_pairs });

    std::cout << G[u].server_name
      << "\t(" << (u - slize)
      << ")\t" << "\t-->\t"
      << G[v].server_name
      << "\t(" << (v - slize) << ")\t"
      << " [" << G[e].qkd_key << "] " << "\n";

    //write in ofstream evaluated data to generate graph/grap hiz graph generated by it's syntax
    dot_file << G[u].server_name << " -> " << G[v].server_name << "[label=\"" << G[e].qkd_key << "\"";

    if (p[v] == u)
      dot_file << ", color=\"black\"";
    else
      dot_file << ", color=\"blue\"";

    dot_file << "]";
  }

  dot_file << "}";

  dot_file.close();

  std::ifstream inFile;

  inFile.open("out_graph.gr");//open the input file

  std::stringstream strStream;

  strStream << inFile.rdbuf();//read the file

  graph_data = strStream.str();//str holds the content of the file

  std::cout << "GRAPH DATA:\n\n" << graph_data << "\n";

  inFile.close();

  /*--------------------------GRAPH SHORTEST PATH FIND END-----------------------*/

}


std::map< int, std::vector < std::pair <std::string, std::string> > >
GraphMapper::get_actual_table()
{
  return actual_pathes;
}


std::string GraphMapper::get_graph()
{
  std::lock_guard <std::mutex> guard(mtx);
  return graph_data;
}


#endif