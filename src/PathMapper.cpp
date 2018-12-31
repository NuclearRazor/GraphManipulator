// PathMapper class
// Author: Ivan Blagopoluchnyy

#include "../src/AdjacencyObjectsGenerator.cpp"
#include "../src/GraphMapper.cpp"
#include "../src/WSServer.cpp"


template <typename T, typename D>
std::ostream& operator<<(std::ostream& os, std::vector< std::pair <T, D> > &lst)
{
  for (const auto &p : lst)
  {
    os << p.first << ", " << p.second << "\n";
  }

  return os;
}


std::string serialize_graph(std::vector <int> _payload)
{

  for (auto &v_el : _payload)
  {
    std::cout << v_el << "\n";
  }

  using namespace std::literals::chrono_literals;

  std::map <int, std::vector < std::pair <std::string, std::string> > > _buf_servers_data = AdjacencyObjectsGenerator(_payload[1], _payload[0], _payload[2]).get_adjency_objects();

  GraphMapper ExecutableTable(_buf_servers_data);

  std::string graph_datastring = ExecutableTable.get_graph();

  std::cout << graph_datastring << "\n";

  std::this_thread::sleep_for(10ms);

  return graph_datastring;

}


int main()
{
    //use with already verifed port
    WSServer(4560);

    return 0;
}
