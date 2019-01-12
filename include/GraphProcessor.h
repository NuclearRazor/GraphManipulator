#ifndef GRAPHPROCESSOR_H
#define GRAPHPROCESSOR_H

class GraphProcessor
{
public:
    GraphProcessor() {};
    GraphProcessor(const GraphProcessor &) {};
    GraphProcessor(GraphProcessor &&) {};
    ~GraphProcessor() {};

    GraphProcessor& operator =(const GraphProcessor &) {};
    GraphProcessor& operator =(GraphProcessor &&) {};

    static std::string serialize_graph(const std::vector <int>& _payload);

};

#endif
