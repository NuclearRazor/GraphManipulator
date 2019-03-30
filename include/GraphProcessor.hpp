#ifndef GRAPHPROCESSOR_HPP
#define GRAPHPROCESSOR_HPP

class GraphProcessor final
{
public:
    GraphProcessor() = default;
    ~GraphProcessor() = default;
    GraphProcessor(const GraphProcessor &) = delete;
    GraphProcessor(GraphProcessor &&) {};
    GraphProcessor& operator =(const GraphProcessor &) = delete;
    GraphProcessor& operator =(GraphProcessor &&) = delete;

    std::string serialize_graph(const std::vector <int>&& _payload);
};

#endif //GRAPHPROCESSOR_HPP
