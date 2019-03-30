// Author: Ivan Blagopoluchnyy
// PathMapper class

#include "AdjacencyObjectsGenerator.cpp"
#include "GraphMapper.cpp"
#include "GraphProcessor.cpp"
#include "WSServer.cpp"

template <typename T, typename D>
inline std::ostream& operator<<(std::ostream& os, std::vector<std::pair <T, D>> &lst)
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
        os << m_iter.first << " ";
        for (const auto &m_node: m_iter.second)
        {
            os << m_node.first << " " << m_node.second << "\n";
        }
    }

    return os;
}

int main()
{
    WSServer(4560);

    return 0;
}
