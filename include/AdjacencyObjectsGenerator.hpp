#ifndef ADJOBJGEN_HPP
#define ADJOBJGEN_HPP

#include <GraphMapper.hpp>

class AdjacencyObjectsGenerator final
{
public:
  AdjacencyObjectsGenerator() = default;
  ~AdjacencyObjectsGenerator() = default;
  AdjacencyObjectsGenerator(const int _adjency_matrix_dim,
                            const int _char_dim,
                            const int _metric_dim);
  AdjacencyObjectsGenerator(AdjacencyObjectsGenerator &&) = default;
  AdjacencyObjectsGenerator(const AdjacencyObjectsGenerator&) = default;
  AdjacencyObjectsGenerator& operator=(const AdjacencyObjectsGenerator&) = default;

  //returns map, where stored generated servers info
  //O(log(n)) average lookup
  graphPayload generate_data();

  //returns map, where stored generated adjency objects
  //O(log(n)) lookup
  graphPayload get_adjency_objects();

private:
  //minimum count of objects set - 2 - see non empty constructor
  unsigned int overall_adjency_matrix_dimension = 0;
  unsigned int characters_dimension = 0;
  unsigned int metric_dimension = 0;
};

#endif //ADJOBJGEN_HPP
