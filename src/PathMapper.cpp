// PathMapper class
// Author: Ivan Blagopoluchnyy

#include "../src/AdjacencyObjectsGenerator.cpp"
#include "../src/GraphMapper.cpp"
#include "../src/GraphProcessor.cpp"
#include "../src/WSServer.cpp"


template <typename T, typename D>
inline std::ostream& operator<<(std::ostream& os, std::vector< std::pair <T, D> > &lst)
{
  for (const auto &p: lst)
  {
    os << p.first << ", " << p.second << "\n";
  }

  return os;
}


template <typename K, typename P, typename S>
inline std::ostream& operator<<(std::ostream& os, std::map <K, std::vector <std::pair <P, S>>> &mlst)
{
    for (const auto &m_iter: mlst)
    {
        std::cout << m_iter.first << " ";

        for (const auto &m_node: m_iter.second)
        {
            std::cout << m_node.first << " " << m_node.second << "\n";
        }

    }

    return os;
}


int main()
{
    WSServer(4560);

    return 0;
}
