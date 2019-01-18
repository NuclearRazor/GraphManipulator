#ifndef ADJOBJGEN_H
#define ADJOBJGEN_H

#include "../include/GraphMapper.h"


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

  /*

  returns map, where stored generated servers info

  */
  //O(log(n)) medium lookup
  std::map <int, std::vector < std::pair <std::string, std::string> >>
  generate_data();

  /*

  returns map, where stored generated adjency objects

  */
  //O(log(n)) lookup
  std::map <int, std::vector < std::pair <std::string, std::string> >>
  get_adjency_objects();

private:

  unsigned int overall_adjency_matrix_dimension = 2; //minimum count of objects set - 2 - see non empty constructor
  unsigned int characters_dimension = 3;
  unsigned int metric_dimension = 5;

};

#endif
