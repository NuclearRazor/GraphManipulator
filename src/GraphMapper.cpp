#ifndef PATHMAPPER_CPP
#define PATHMAPPER_CPP

#include "../include/GraphMapper.h"


GraphMapper::GraphMapper(const graphPayload &&servers_data)
{
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

  graphPayload::iterator it_table_data;

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

  /*--------------------------FIND SHORTEST PATH BY DJKSTRA ALGORITHM START-----------------------*/

  typedef boost::graph_traits < ServersGraph >::vertex_iterator Viter;

  //START POINT IN DJKSTRA ALGORITHM
  Viter initial = boost::vertices(G).first;

  typedef boost::graph_traits < ServersGraph >::vertex_descriptor Vertex;
  Vertex s = 0;

  if(!(*initial)) 
  {
      Vertex s = *initial;
  }
  else
  {
      throw std::runtime_error("Cannot get value of first vertex");
  }

  std::vector <Vertex> predecessors(boost::num_vertices(G)); // store parents nodes
  typedef int Weight;
  std::vector <Weight> distances(boost::num_vertices(G)); // store distances

  if (!(s))
      std::cout << "Warning: initial vertex is equal zero\n";

  //[1] parameter -> graph
  //[2] parameter -> start point (selected vertex)
  //[3] parameter -> graph map properties

  //create automatically propery map by boost iterator
  //https://svn.boost.org/trac10/changeset/82439
  boost::dijkstra_shortest_paths(G, s, 
      boost::predecessor_map(boost::make_iterator_property_map(predecessors.begin(), boost::get(boost::vertex_index, G)))
      .distance_map(boost::make_iterator_property_map(distances.begin(), boost::get(boost::vertex_index, G))));

  /*--------------------------FIND SHORTEST PATH BY DJKSTRA ALGORITHM END-----------------------*/

  std::stringstream graph_stream;
  graph_stream << "digraph D {\n"
      << "  rankdir=LR\n"
      << "  size=\"4,3\"\n"
      << "  ratio=\"fill\"\n"
      << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

  //store vertices integer indexes
  std::vector <Vertex> p(boost::num_vertices(G));

  boost::graph_traits <ServersGraph>::edge_iterator ei, ei_end;

  using VD = ServersGraph::vertex_descriptor;

  VD start_vertex = boost::num_vertices(G);
  VD end_vertex = boost::num_vertices(G);

  auto predmap = predecessors.data(); // interior properties: boost::get(boost::vertex_predecessor, g);
  auto distmap = distances.data();

  size_t distance = distmap[end_vertex];

  if (distance != size_t(-1))
  {
    std::deque<VD> path;

    for (VD current = end_vertex; current != G.null_vertex() && predmap[current] != current && current != start_vertex;)
    {
      path.push_front(predmap[current]);
      current = predmap[current];
    }
    std::copy(path.begin(), path.end(), std::ostream_iterator<VD>(std::cout, ", "));
  }


  std::vector <ServersGraph::vertex_descriptor> pr_map(boost::num_vertices(G));
  std::vector <double>                   dist_map(boost::num_vertices(G));

  //for (auto vd : boost::make_iterator_range(boost::vertices(G)))
  //{
  //  std::cout << "distance(" << vd << ") = " << dist_map[vd] << ", ";
  //  std::cout << "parent(" << vd << ") = " << pr_map[vd] << std::endl;
  //}


  //slize to print out current index of vertex
  size_t slize = 2 * nodes_count + 1;

  graphPayload actual_pathes;
  std::vector < std::pair <std::string, std::string> > _buf_pairs;

  //print out path table and store it to .dot file
  for (std::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei)
  {
    boost::graph_traits < ServersGraph >::edge_descriptor e = *ei;
    boost::graph_traits < ServersGraph >::vertex_descriptor u = boost::source(e, G), v = boost::target(e, G);

    //store all finded pathes to public map
    _buf_pairs.push_back(std::make_pair(G[u].server_name, G[v].server_name));
    actual_pathes.insert({ G[e].qkd_key, _buf_pairs });

    //write into stringstream evaluated data as graphiz syntax string
    graph_stream << G[u].server_name << " -> " << G[v].server_name << "[label=\"" << G[e].qkd_key << "\"";

    if (p[v] == u)
        graph_stream << ", color=\"black\"";
    else
        graph_stream << ", color=\"blue\"";

    graph_stream << "]";
  }

  graph_stream << "}";
  graph_data = graph_stream.str();//str holds the content of the file

  /*--------------------------GRAPH SHORTEST PATH FIND END-----------------------*/

}


graphPayload
GraphMapper::get_actual_table()
{
  return actual_pathes;
}


std::string GraphMapper::get_graph()
{
  return graph_data;
}


#endif