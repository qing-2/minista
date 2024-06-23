#ifndef ADJACENCY_LIST_GRAPH_H
#define ADJACENCY_LIST_GRAPH_H

#include "Graph.h"
#include "Path.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class AdjacencyListGraph : public Graph {
private:
    std::unordered_map<int, Vertex> vertices;
    std::unordered_map<int, std::vector<Edge>> adjacencyList;

    void findAllPathsUtil(int u, int d, std::unordered_set<int>& visited, Path& path, std::vector<Path>& paths, int maxPathsPerEnd, int& totalPaths, int maxTotalPaths) const;

public:
    void addVertex(const Vertex& vertex) override;
    void addEdge(const Edge& edge) override;
    void loadFromFile(const std::string& filename) override;
    void printGraph() const override;
    void breakLoop() override;
    bool breakCycleUtil(int v, std::unordered_set<int>& visited, std::unordered_set<int>& recStack);

    std::vector<Path> findAllPaths(int maxPathsPerEnd, int maxTotalPaths) const override;
    ~AdjacencyListGraph();
};

#endif // ADJACENCY_LIST_GRAPH_H
